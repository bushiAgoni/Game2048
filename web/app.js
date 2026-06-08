const SIZE = 4;
const WIN_TILE = 2048;
const BEST_KEY = "game2048-web-best-score";

const boardElement = document.getElementById("board");
const scoreElement = document.getElementById("score");
const bestScoreElement = document.getElementById("bestScore");
const movesElement = document.getElementById("moves");
const maxTileElement = document.getElementById("maxTile");
const progressFillElement = document.getElementById("progressFill");
const statusElement = document.getElementById("status");

let grid = [];
let score = 0;
let bestScore = Number(localStorage.getItem(BEST_KEY) || 0);
let moves = 0;
let hasWon = false;
let isGameOver = false;
let lastSpawnKey = "";
let mergedKeys = new Set();

function createEmptyGrid() {
    return Array.from({ length: SIZE }, () => Array(SIZE).fill(0));
}

function resetGame() {
    grid = createEmptyGrid();
    score = 0;
    moves = 0;
    hasWon = false;
    isGameOver = false;
    mergedKeys = new Set();
    spawnTile();
    spawnTile();
    render("已生成初始棋盘");
}

function getEmptyCells() {
    const cells = [];

    for (let row = 0; row < SIZE; row += 1) {
        for (let col = 0; col < SIZE; col += 1) {
            if (grid[row][col] === 0) {
                cells.push({ row, col });
            }
        }
    }

    return cells;
}

function spawnTile() {
    const emptyCells = getEmptyCells();

    if (emptyCells.length === 0) {
        return;
    }

    const index = Math.floor(Math.random() * emptyCells.length);
    const { row, col } = emptyCells[index];
    grid[row][col] = Math.random() < 0.9 ? 2 : 4;
    lastSpawnKey = `${row}-${col}`;
}

function mergeLine(line) {
    const numbers = line.filter((value) => value !== 0);
    const result = [];
    const mergedIndexes = [];
    let gainedScore = 0;

    for (let index = 0; index < numbers.length; index += 1) {
        if (index + 1 < numbers.length && numbers[index] === numbers[index + 1]) {
            const combined = numbers[index] * 2;
            result.push(combined);
            mergedIndexes.push(result.length - 1);
            gainedScore += combined;
            index += 1;
        } else {
            result.push(numbers[index]);
        }
    }

    while (result.length < SIZE) {
        result.push(0);
    }

    return { result, gainedScore, mergedIndexes };
}

function move(direction) {
    if (isGameOver) {
        render("本局已经结束，请重新开始");
        return;
    }

    const previous = grid.map((row) => row.slice());
    let gainedScore = 0;
    const nextMergedKeys = new Set();

    if (direction === "left" || direction === "right") {
        for (let row = 0; row < SIZE; row += 1) {
            const source = direction === "right" ? grid[row].slice().reverse() : grid[row].slice();
            const merged = mergeLine(source);
            const finalLine = direction === "right" ? merged.result.slice().reverse() : merged.result;
            grid[row] = finalLine;
            gainedScore += merged.gainedScore;

            merged.mergedIndexes.forEach((index) => {
                const col = direction === "right" ? SIZE - 1 - index : index;
                nextMergedKeys.add(`${row}-${col}`);
            });
        }
    }

    if (direction === "up" || direction === "down") {
        for (let col = 0; col < SIZE; col += 1) {
            const column = [];
            for (let row = 0; row < SIZE; row += 1) {
                column.push(grid[row][col]);
            }

            const source = direction === "down" ? column.slice().reverse() : column;
            const merged = mergeLine(source);
            const finalColumn = direction === "down" ? merged.result.slice().reverse() : merged.result;

            for (let row = 0; row < SIZE; row += 1) {
                grid[row][col] = finalColumn[row];
            }

            gainedScore += merged.gainedScore;
            merged.mergedIndexes.forEach((index) => {
                const row = direction === "down" ? SIZE - 1 - index : index;
                nextMergedKeys.add(`${row}-${col}`);
            });
        }
    }

    const changed = hasGridChanged(previous, grid);
    if (!changed) {
        mergedKeys = new Set();
        lastSpawnKey = "";
        render("这一步没有改变棋盘");
        return;
    }

    score += gainedScore;
    moves += 1;
    bestScore = Math.max(bestScore, score);
    localStorage.setItem(BEST_KEY, String(bestScore));
    mergedKeys = nextMergedKeys;
    lastSpawnKey = "";
    spawnTile();

    const message = buildStatusMessage(gainedScore);
    checkGameState(message);
}

function hasGridChanged(before, after) {
    for (let row = 0; row < SIZE; row += 1) {
        for (let col = 0; col < SIZE; col += 1) {
            if (before[row][col] !== after[row][col]) {
                return true;
            }
        }
    }

    return false;
}

function buildStatusMessage(gainedScore) {
    if (gainedScore > 0) {
        return `漂亮，合并得分 +${gainedScore}`;
    }

    return "移动成功，已生成一个新数字";
}

function checkGameState(message) {
    const maxTile = getMaxTile();
    let nextMessage = message;

    if (maxTile >= WIN_TILE && !hasWon) {
        hasWon = true;
        nextMessage = "达成 2048，可以继续挑战更高分";
    }

    if (!canMove()) {
        isGameOver = true;
        nextMessage = "棋盘已满且没有可合并数字，本局结束";
    }

    render(nextMessage);
}

function canMove() {
    if (getEmptyCells().length > 0) {
        return true;
    }

    for (let row = 0; row < SIZE; row += 1) {
        for (let col = 0; col < SIZE; col += 1) {
            if (col + 1 < SIZE && grid[row][col] === grid[row][col + 1]) {
                return true;
            }
            if (row + 1 < SIZE && grid[row][col] === grid[row + 1][col]) {
                return true;
            }
        }
    }

    return false;
}

function getMaxTile() {
    return grid.flat().reduce((max, value) => Math.max(max, value), 0);
}

function getTileClass(value) {
    if (value > WIN_TILE) {
        return "tile-super";
    }

    return `tile-${value}`;
}

function render(message) {
    boardElement.innerHTML = "";

    for (let row = 0; row < SIZE; row += 1) {
        for (let col = 0; col < SIZE; col += 1) {
            const cell = document.createElement("div");
            cell.className = "cell";

            const value = grid[row][col];
            if (value > 0) {
                const tile = document.createElement("div");
                const key = `${row}-${col}`;
                tile.className = `tile ${getTileClass(value)}`;
                tile.dataset.digits = String(value).length;
                tile.textContent = value;

                if (key === lastSpawnKey) {
                    tile.classList.add("new");
                }

                if (mergedKeys.has(key)) {
                    tile.classList.add("merged");
                }

                cell.appendChild(tile);
            }

            boardElement.appendChild(cell);
        }
    }

    const maxTile = getMaxTile();
    const progress = Math.min(100, (Math.log2(Math.max(maxTile, 2)) / Math.log2(WIN_TILE)) * 100);

    scoreElement.textContent = score;
    bestScoreElement.textContent = bestScore;
    movesElement.textContent = moves;
    maxTileElement.textContent = maxTile;
    progressFillElement.style.width = `${progress}%`;
    statusElement.textContent = message;
}

function bindEvents() {
    document.querySelectorAll("[data-direction]").forEach((button) => {
        button.addEventListener("click", () => move(button.dataset.direction));
    });

    document.getElementById("restart").addEventListener("click", resetGame);
    document.getElementById("restartTop").addEventListener("click", resetGame);

    document.addEventListener("keydown", (event) => {
        const map = {
            ArrowUp: "up",
            ArrowDown: "down",
            ArrowLeft: "left",
            ArrowRight: "right",
            w: "up",
            W: "up",
            s: "down",
            S: "down",
            a: "left",
            A: "left",
            d: "right",
            D: "right"
        };

        if (event.key === "r" || event.key === "R") {
            resetGame();
            return;
        }

        if (map[event.key]) {
            event.preventDefault();
            move(map[event.key]);
        }
    });
}

bindEvents();
resetGame();
