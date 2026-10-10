# Filesystem helpers for the fullstack launcher.
function Resolve-FullPath([string]$Path) {
    if ([string]::IsNullOrWhiteSpace($Path)) {
        return $Path
    }

    $item = Get-Item -LiteralPath $Path -ErrorAction SilentlyContinue
    if ($null -ne $item) {
        return $item.FullName
    }

    $parent = Split-Path -Parent $Path
    if ([string]::IsNullOrWhiteSpace($parent)) {
        $parent = "."
    }
    $leaf = Split-Path -Leaf $Path
    $resolvedParent = (Resolve-Path $parent).Path
    return (Join-Path $resolvedParent $leaf)
}

function Ensure-Directory([string]$Path) {
    if (!(Test-Path -LiteralPath $Path)) {
        New-Item -ItemType Directory -Path $Path -Force | Out-Null
    }
    return (Resolve-FullPath $Path)
}

function Copy-DirectoryContents([string]$Source, [string]$Destination) {
    if (!(Test-Path -LiteralPath $Source)) {
        return
    }

    $sourceFull = Resolve-FullPath $Source
    $destinationFull = Ensure-Directory $Destination
    if ($sourceFull.TrimEnd('\') -ieq $destinationFull.TrimEnd('\')) {
        return
    }

    Get-ChildItem -LiteralPath $sourceFull -Force | ForEach-Object {
        Copy-Item -LiteralPath $_.FullName -Destination $destinationFull -Recurse -Force
    }
}

function Copy-FileIfExists([string]$Source, [string]$DestinationDir) {
    if (Test-Path -LiteralPath $Source) {
        Ensure-Directory $DestinationDir | Out-Null
        $destination = Join-Path $DestinationDir (Split-Path -Leaf $Source)
        if ((Resolve-FullPath $Source).TrimEnd('\') -ieq (Resolve-FullPath $destination).TrimEnd('\')) {
            return
        }
        Copy-Item -LiteralPath $Source -Destination $destination -Force
    }
}

function Copy-FileToPathIfDifferent([string]$Source, [string]$Destination) {
    if (!(Test-Path -LiteralPath $Source)) {
        return
    }

    $parent = Split-Path -Parent $Destination
    Ensure-Directory $parent | Out-Null
    if ((Resolve-FullPath $Source).TrimEnd('\') -ieq (Resolve-FullPath $Destination).TrimEnd('\')) {
        return
    }

    Copy-Item -LiteralPath $Source -Destination $Destination -Force
}

function Write-TextFile([string]$Path, [string]$Text) {
    $parent = Split-Path -Parent $Path
    Ensure-Directory $parent | Out-Null
    # [System.IO.File]::WriteAllText 默认写 UTF-8 without BOM，
    # 避免 BOM 污染 conf 文件导致 C++ 读取时 base_url 带不可见字符。
    [System.IO.File]::WriteAllText($Path, $Text, [System.Text.Encoding]::UTF8)
}

function Write-PidFile([string]$Path, [object]$Data) {
    $json = $Data | ConvertTo-Json -Depth 5
    Write-TextFile -Path $Path -Text $json
}
