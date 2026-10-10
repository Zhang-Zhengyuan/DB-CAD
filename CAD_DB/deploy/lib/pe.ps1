# Read PE imports without dumpbin or a Visual Studio shell. Used before launching
# so missing DLLs become actionable script errors instead of Windows dialogs.
function Get-PeImports([string]$Path) {
    $stream = [System.IO.File]::OpenRead($Path)
    $reader = New-Object System.IO.BinaryReader($stream)
    try {
        $stream.Position = 0x3c
        $peOffset = $reader.ReadInt32()
        $stream.Position = $peOffset
        if ($reader.ReadUInt32() -ne 0x4550) { throw "Invalid PE file: $Path" }
        $machine = $reader.ReadUInt16()
        if ($machine -ne 0x8664) { throw "Expected x64 binary: $Path (machine=$machine)" }
        $sections = $reader.ReadUInt16()
        $stream.Position = $peOffset + 20
        $optionalSize = $reader.ReadUInt16()
        $optionalOffset = $peOffset + 24
        $stream.Position = $optionalOffset
        if ($reader.ReadUInt16() -ne 0x20b) { throw "Expected PE32+ binary: $Path" }
        $stream.Position = $optionalOffset + 120
        $importsRva = $reader.ReadUInt32()
        $sectionTable = @()
        for ($i = 0; $i -lt $sections; $i++) {
            $stream.Position = $optionalOffset + $optionalSize + 40 * $i + 8
            $virtualSize = $reader.ReadUInt32()
            $rva = $reader.ReadUInt32()
            $rawSize = $reader.ReadUInt32()
            $raw = $reader.ReadUInt32()
            $sectionTable += @{ Rva = $rva; Size = [Math]::Max($virtualSize, $rawSize); Raw = $raw }
        }
        function Convert-Rva([uint32]$Value) {
            foreach ($section in $sectionTable) {
                if ($Value -ge $section.Rva -and $Value -lt ($section.Rva + $section.Size)) {
                    return [long]($Value - $section.Rva + $section.Raw)
                }
            }
            throw "Invalid PE RVA in $Path"
        }
        if ($importsRva -eq 0) { return }
        $importsOffset = Convert-Rva $importsRva
        for ($i = 0; $i -lt 4096; $i++) {
            $stream.Position = $importsOffset + 20 * $i + 12
            $nameRva = $reader.ReadUInt32()
            if ($nameRva -eq 0) { break }
            $stream.Position = Convert-Rva $nameRva
            $bytes = New-Object 'System.Collections.Generic.List[byte]'
            while (($value = $reader.ReadByte()) -ne 0) { $bytes.Add($value) }
            [System.Text.Encoding]::ASCII.GetString($bytes.ToArray())
        }
    } finally { $reader.Dispose(); $stream.Dispose() }
}

function Assert-ClientRuntime([string]$ExePath) {
    $root = Split-Path -Parent $ExePath
    $imports = @(Get-PeImports $ExePath)
    $isDebug = $imports -contains 'Qt6Cored.dll'
    $platform = Join-Path $root $(if ($isDebug) { 'platforms\qwindowsd.dll' } else { 'platforms\qwindows.dll' })
    if (-not (Test-Path -LiteralPath $platform)) { throw "Qt platform plugin missing: $platform. Run packaging with the Qt kit used for this build." }
    $queue = New-Object 'System.Collections.Generic.Queue[string]'
    $queue.Enqueue($ExePath)
    # Validate Qt plugins too, including their transitive imports.
    foreach ($folder in @('platforms','imageformats','iconengines','styles','tls','networkinformation','generic')) {
        $dir = Join-Path $root $folder
        if (Test-Path -LiteralPath $dir) { Get-ChildItem -LiteralPath $dir -Filter '*.dll' -File | ForEach-Object { $queue.Enqueue($_.FullName) } }
    }
    $seen = @{}
    $missing = @()
    while ($queue.Count -gt 0) {
        $file = $queue.Dequeue()
        if ($seen.ContainsKey($file)) { continue }
        $seen[$file] = $true
        foreach ($name in @(Get-PeImports $file)) {
            if ($name -match '^(api-ms-|ext-ms-)') { continue } # Windows API sets
            $local = Join-Path $root $name
            if (Test-Path -LiteralPath $local) { $queue.Enqueue($local); continue }
            # Qt/CAD runtime must be self-contained; system DLLs/VC runtime may
            # resolve via System32, but never via a developer's PATH.
            if ($name -notmatch '^(Qt[56]|SpaACIS)' -and (Test-Path -LiteralPath (Join-Path "$env:WINDIR\System32" $name))) { continue }
            $missing += "$name (required by $(Split-Path -Leaf $file))"
        }
    }
    if ($missing.Count) { throw "Client runtime is incomplete:`n$($missing | Sort-Object -Unique | Out-String)" }
    Write-Host "[OK] x64 client runtime imports and Qt plugins verified ($($seen.Count) binaries)."
}
