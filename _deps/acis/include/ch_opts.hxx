#ifndef CH_OPTS_HXX
#define CH_OPTS_HXX

#include "scheme.hxx"
#include "logical.h"

class convex_hull_options;

logical is_Scm_Convex_Hull_Opt(ScmObject r);
convex_hull_options *get_Scm_Convex_Hull_Opt(ScmObject r);
ScmObject make_Scm_Convex_Hull_Opt(convex_hull_options *ch_opt);

#endif
