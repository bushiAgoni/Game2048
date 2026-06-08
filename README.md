# 2048 控制台游戏

## 项目简介

本项目是一个基于 C++17 的 2048 控制台版游戏，适合作为“C++ 面向对象程序设计”课程设计提交。项目不使用 EasyX 或其他第三方图形库，重点展示类封装、职责划分、对象协作和核心算法复用。

## 功能特点

- 4 x 4 棋盘，初始随机生成两个数字。
- 支持 W/A/S/D 控制上下左右移动。
- 支持 R 重新开始，Q 退出游戏。
- 相同数字按移动方向合并，每个格子一次移动最多合并一次。
- 有效移动后才随机生成新数字，无效移动不会生成新数字。
- 实时显示当前分数、最高分和操作提示。
- 出现 2048 时提示胜利，棋盘已满且无法移动时提示失败。
- 提供多文件版和单文件整合版，便于课堂提交和快速运行。

## 开发环境

- 操作系统：Windows
- 编辑器：VSCode
- 编译器：g++ / MinGW-w64
- 语言标准：C++17 或以上
- 项目类型：C++ 面向对象控制台游戏

## 项目结构

```text
Game2048/
|-- include/
|   |-- Board.h
|   |-- Game.h
|   |-- InputHandler.h
|   |-- RandomGenerator.h
|   |-- Renderer.h
|   `-- ScoreManager.h
|-- src/
|   |-- Board.cpp
|   |-- Game.cpp
|   |-- InputHandler.cpp
|   |-- RandomGenerator.cpp
|   |-- Renderer.cpp
|   |-- ScoreManager.cpp
|   `-- main.cpp
|-- docs/
|   |-- 项目设计文档.md
|   |-- 测试报告.md
|   `-- 答辩讲解提纲.md
|-- web/
|   |-- index.html
|   |-- styles.css
|   |-- app.js
|   `-- serve.cjs
|-- single_file/
|   `-- main.cpp
`-- README.md
```

## 文件作用说明

| 文件 | 作用 |
|---|---|
| `include/Game.h`, `src/Game.cpp` | 游戏主控制类，负责主循环、输入处理、胜负判断、重新开始和退出。 |
| `include/Board.h`, `src/Board.cpp` | 棋盘类，负责 4 x 4 数据存储、移动、合并、随机生成数字和可移动判断。 |
| `include/Renderer.h`, `src/Renderer.cpp` | 控制台显示类，负责打印棋盘、分数、提示信息、胜利和失败提示。 |
| `include/InputHandler.h`, `src/InputHandler.cpp` | 输入处理类，负责读取输入、判断合法性、转换为游戏命令和方向。 |
| `include/ScoreManager.h`, `src/ScoreManager.cpp` | 分数管理类，负责当前分数和运行期最高分。 |
| `include/RandomGenerator.h`, `src/RandomGenerator.cpp` | 随机数类，负责随机位置和 2/4 数字生成。 |
| `src/main.cpp` | 多文件版程序入口。 |
| `single_file/main.cpp` | 单文件整合版，可以单独复制编译运行。 |
| `web/index.html`, `web/styles.css`, `web/app.js` | 彩色按钮交互页面版，支持鼠标点击方向键操作。 |
| `docs/项目设计文档.md` | 完整中文设计文档，包含需求、设计、算法、流程图、UML、测试和总结。 |
| `docs/测试报告.md` | 独立测试报告，便于提交或截图。 |
| `docs/答辩讲解提纲.md` | 课程答辩汇报提纲。 |

## 编译方法

在 VSCode 终端进入项目根目录：

```bash
cd Game2048
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o 2048.exe
```

单文件版编译：

```bash
cd Game2048
g++ -std=c++17 -Wall -Wextra single_file/main.cpp -o 2048_single.exe
```

## 运行方法

多文件版：

```bash
./2048.exe
```

单文件版：

```bash
./2048_single.exe
```

网页交互版：

直接打开 `web/index.html`，即可使用页面上的方向按钮进行操作。

如果浏览器限制直接打开本地文件，也可以用本地预览服务：

```bash
node web/serve.cjs
```

然后访问 `http://127.0.0.1:5500/`。

如果没有 Node，也可以使用 Python：

```bash
cd web
py -m http.server 5500 --bind 127.0.0.1
```

## 操作说明

| 输入 | 功能 |
|---|---|
| `W` 或 `w` | 上移 |
| `A` 或 `a` | 左移 |
| `S` 或 `s` | 下移 |
| `D` 或 `d` | 右移 |
| `R` 或 `r` | 重新开始 |
| `Q` 或 `q` | 退出游戏 |

## 游戏规则

1. 棋盘大小为 4 x 4。
2. 初始棋盘随机生成两个数字，数字为 2 或 4。
3. 每次有效移动后，系统在空白格随机生成一个新数字。
4. 相同数字沿移动方向相邻时合并，合并后的数值加入当前分数。
5. 每个格子在一次移动中最多参与一次合并。
6. 棋盘中出现 2048 时提示胜利。
7. 棋盘没有空格且不存在相邻可合并数字时游戏失败。

## 面向对象设计说明

项目将游戏拆分为六个主要类：

- `Game`：协调其他对象，管理主循环和游戏状态。
- `Board`：封装棋盘数据和移动合并算法。
- `Renderer`：封装控制台输出。
- `InputHandler`：封装输入解析。
- `ScoreManager`：封装分数管理。
- `RandomGenerator`：封装随机数生成。

这种设计避免把所有逻辑写在 `main()` 中，使代码更清晰，也便于测试和扩展。

## 测试说明

已使用以下命令完成编译检查：

```bash
g++ -std=c++17 -Wall -Wextra -Iinclude src/*.cpp -o 2048.exe
g++ -std=c++17 -Wall -Wextra single_file/main.cpp -o 2048_single.exe
```

测试重点覆盖启动、随机初始棋盘、四方向移动、数字合并、分数更新、无效输入、重新开始、退出、胜利判断和失败判断。详细测试用例见 `docs/测试报告.md`。

## 常见运行问题

| 问题 | 解决方法 |
|---|---|
| `g++` 不是内部或外部命令 | 安装 MinGW-w64，并把 `bin` 目录加入系统 Path。 |
| 中文显示乱码 | 建议使用 Windows Terminal 或 VSCode 终端运行；程序已在 Windows 下设置 UTF-8 控制台编码。 |
| `src/*.cpp` 无法展开 | 在 PowerShell、Git Bash 或 MSYS2 终端中执行推荐命令；如果终端不支持通配符，可手动列出所有 `.cpp` 文件。 |
| 运行 `./2048.exe` 提示找不到文件 | 先确认编译成功，并且当前终端目录位于 `Game2048`。 |

## 后续改进方向

- 增加最高分本地文件保存功能。
- 增加撤销一步功能。
- 增加不同棋盘大小选择。
- 增加 EasyX 图形界面或跨平台 GUI 版本。
- 增加自动化单元测试入口，便于验证移动合并算法。
