"""Exercise the exact inline GitHub policy without network or extra packages.

Run from repository root: python scripts/tests/test-pr-policy.py
Requires Node.js. No PR code is loaded by the production workflow.
"""
import json
from pathlib import Path
import subprocess
import textwrap
import unittest


ROOT = Path(__file__).resolve().parents[2]
WORKFLOW = ROOT / '.github/workflows/pr-policy.yml'
POLICY = textwrap.dedent(WORKFLOW.read_text(encoding='utf-8').split('script: |\n', 1)[1])
HARNESS = r'''
const fs = require('node:fs');
const input = JSON.parse(fs.readFileSync(0, 'utf8'));
const failures = [];
let calls = 0;
const context = {
  repo: {owner: 'team', repo: 'cad'},
  payload: {pull_request: {
    head: {ref: input.branch}, title: input.title, body: input.body
  }}
};
const github = {rest: {issues: {get: async args => {
  calls++;
  if (args.owner !== 'team' || args.repo !== 'cad' || args.issue_number !== 123)
    throw new Error('Wrong repository or issue');
  if (input.status) throw {status: input.status};
  return {data: input.issue};
}}}};
const core = {setFailed: message => failures.push(message), info: () => {}};
const AsyncFunction = Object.getPrototypeOf(async function(){}).constructor;
(async () => {
  await new AsyncFunction('context', 'github', 'core', input.script)(context, github, core);
  process.stdout.write(JSON.stringify({failures, calls}));
})().catch(error => { console.error(error); process.exit(1); });
'''


class PolicyTests(unittest.TestCase):
    def run_case(self, expected, **overrides):
        data = dict(script=POLICY, branch='fix/123-pull-conflict',
                    title='fix(collab): protect local changes', body='Closes #123',
                    issue={'state': 'open'})
        data.update(overrides)
        result = subprocess.run(['node', '-e', HARNESS], input=json.dumps(data),
                                text=True, encoding='utf-8', capture_output=True, check=True)
        parsed = json.loads(result.stdout)
        self.assertEqual(not parsed['failures'], expected, parsed)
        return parsed

    def test_accepts_valid_cases(self):
        for kind in ('feat', 'fix', 'docs', 'refactor', 'test', 'chore', 'perf', 'build', 'ci', 'revert'):
            with self.subTest(kind=kind):
                result = self.run_case(True, branch=f'{kind}/123-task-name', title=f'{kind}: 有效说明')
                self.assertEqual(result['calls'], 1)
        self.run_case(True, body='## Issue\r\n\r\nCloses #123\r\n\r\nDetails')

    def test_rejects_invalid_branches_before_api(self):
        for branch in ('master', 'docs/local-ai-governance', 'fix/no-number',
                       'feature/123-name', 'fix/0-name', 'fix/0123-name',
                       'fix/123-UPPER', 'fix/123-bad--name', 'fix/123-name/extra'):
            with self.subTest(branch=branch):
                result = self.run_case(False, branch=branch)
                self.assertEqual(result['calls'], 0)

    def test_rejects_bad_titles(self):
        for title in ('fix bug', 'feat: wrong type', 'fix: ', 'fix(): bad scope'):
            with self.subTest(title=title):
                self.run_case(False, title=title)

    def test_rejects_bad_links(self):
        for body in (None, '', 'Closes #124', 'Closes #0123', 'Closes #123 extra',
                     'Closes #123\nCloses #123', 'Closes #123\nCloses #124',
                     'Closes other/repo#123', '<!-- Closes #123 -->'):
            with self.subTest(body=body):
                self.run_case(False, body=body)

    def test_rejects_pr_closed_issue_and_api_errors(self):
        self.run_case(False, issue={'state': 'open', 'pull_request': {}})
        self.run_case(False, issue={'state': 'closed'})
        for status in (403, 404, 500):
            self.run_case(False, status=status)


if __name__ == '__main__':
    unittest.main(verbosity=2)
