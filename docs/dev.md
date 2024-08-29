# 开发环境配置

## vs2022

+ 需要登录vs才可以安装扩展
+ 安装 vs后再退出 qt tool扩展才会安装，安装然后选中版本。自动配置的json文件需要重新生成.此时才会生效。

## qt

## vscode

+ cmake 工具 需要选择预设工具包使用vs2022 否则无法使用标准库等工具
+ qt 工具 需要配置qt kit目录
+ cmake文件中需要指定 Qt Kit Dir
```
set(CMAKE_PREFIX_PATH "e:/Qt/6.7.2/msvc2019_64") # Qt Kit Dir
```
+ 配置文件 .vscode/setting.json  中修改生成目录
```
{
    "qtConfigure.qtKitDir": "e:/Qt/6.7.2/msvc2019_64",
    "cmake.buildDirectory": "${workspaceFolder}/build/vscode",
    "editor.fontFamily": "Hack,Consolas, 'Courier New', monospace"
}
```
**备注：调式启动选择QtBuild**


## Git

Git 会默认拒绝推送到当前检出的分支。
+ 临时更改: git config receive.denyCurrentBranch warn
+ 永久更改: 

    git config --local receive.denyCurrentBranch warn
    
    将 warn 改为 ignore 或 update 也可以。
