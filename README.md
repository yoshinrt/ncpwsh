# ncpwsh

ncpwsh is a tool to run PowerShell scripts without showing the console window.

## How to use

1. Save the PowerShell script you want to run, for example as `c:\foo\bar.ps1`.
1. Copy `ncpwsh.exe` to `c:\foo\bar.exe`.
1. Run `bar.exe` to execute `bar.ps1` without showing the console window.

## Benefits

- The console window is not displayed, so it does not clutter the screen.
- Can be used as an .exe file, allowing for extension association and D&D target usage.

## Notes

You can save disk space by creating symbolic links using the `mklink` command instead of copying multiple `ncpwsh.exe` files.

<hr/>

ncpwsh は、PowerShell スクリプトをコンソールウィンドウを表示せずに実行するためのツールです。

## 使い方

1. 実行したい PowerShell スクリプトを `c:\foo\bar.ps1` のように保存します。
1. `ncpwsh.exe` を `c:\foo\bar.exe` にコピーします。
1. `bar.exe` を実行すると、コンソールウィンドウを表示することなく、`bar.ps1` が実行されます。

## メリット

- コンソールウィンドウが表示されないため、画面が邪魔になりません。
- .exe ファイルとして実行できるため、拡張子関連付けや D&D のターゲットとして使用できます。

## 補足

複数の `ncpwsh.exe` をコピーする代わりに、`mklink` コマンドを使用してシンボリックリンクを作成することで、ディスク容量を節約できます。
