import * as fs from "fs";
import puppeteer from "puppeteer";
import Solver from "./Solver";
import LogBuilder from "../../src/service/logger/LogBuilder";

export default class QueenSolver {
    solver: Solver;

    constructor(solver: Solver) {
        this.solver = solver;
    }

    async solvePuzzle(): Promise<void> {
        const browser = await puppeteer.launch({
            headless: false,
            defaultViewport: null,
        });

        // ?????
        // @ts-ignore
        await browser.setCookie({
            domain: ".www.linkedin.com",
            name: "bscookie",
            value: '"x',
            httpOnly: true,
            secure: true,
            sameSite: "None",
            expires: -1,
            path: "/",
            session: false,
        }, {
            domain: ".www.linkedin.com",
            name: "JSESSIONID",
            value: 'x',
            httpOnly: false,
            secure: true,
            sameSite: "None",
            expires: -1,
            path: "/",
            session: false,
        }, {
            domain: ".www.linkedin.com",
            name: "timezone",
            value: "Europe/Vienna",
            httpOnly: false,
            secure: true,
            sameSite: "None",
            expires: -1,
            path: "/",
            session: false,
        }, {
            domain: ".www.linkedin.com",
            name: "li_theme_set",
            value: "app",
            httpOnly: false,
            secure: true,
            sameSite: "None",
            expires: -1,
            path: "/",
            session: false,
        }, {
            domain: ".www.linkedin.com",
            name: "li_theme",
            value: "light",
            httpOnly: false,
            secure: true,
            sameSite: "None",
            expires: -1,
            path: "/",
            session: false,
        }, {
            domain: ".www.linkedin.com",
            name: "li_at",
            value: "x",
            httpOnly: false,
            secure: true,
            sameSite: "None",
            expires: -1,
            path: "/",
            session: false,
        }, {
            domain: ".www.linkedin.com",
            name: "li_rm",
            value: "x",
            httpOnly: false,
            secure: true,
            sameSite: "None",
            expires: -1,
            path: "/",
            session: false,
        });

        const mainPage = await browser.newPage();
        await mainPage.setBypassCSP(true);

        await QueenSolver.sleep(1000);

        await mainPage.evaluateOnNewDocument(() => {
            localStorage.setItem("play:tutorial:queensv2", "true");
        });
        await mainPage.goto("https://www.linkedin.com/games/view/queens/desktop");

        try {
            LogBuilder
                .start()
                .level("INFO")
                .info("LinkedIn.GameSolver", "QueenSolver")
                .line("Waiting for game board")
                .done();

            await mainPage.waitForSelector(".pr-game-web__main-container.pr-game-web__main-container--games .queens-board:not(.grid-board--disabled):not(.tutorial-board)", {timeout: 5000});
        }
        catch {
            try {
                LogBuilder
                    .start()
                    .level("INFO")
                    .info("LinkedIn.GameSolver", "QueenSolver")
                    .line("Waiting for game board failed. Waiting for launch button.")
                    .done();

                await mainPage.waitForSelector(".launch-footer>button", {timeout: 5000})
                    .then(button => {
                        return button.click()
                            .then(() => mainPage.waitForSelector(
                                "[role='dialog']>button",
                                {timeout: 100})
                                .then(btn => btn.click())
                                .catch(() => {}),
                            )
                            .then(() => mainPage.waitForSelector(
                                ".pr-game-web__main-container.pr-game-web__main-container--games .queens-board:not(.grid-board--disabled):not(.tutorial-board)",
                                {timeout: 5000},
                            ));
                    });
            }
            catch(err) {
                LogBuilder
                    .start()
                    .level("ERROR")
                    .info("LinkedIn.GameSolver", "QueenSolver")
                    .line("Everything failed. Could not solve todays game.")
                    .object("error", err)
                    .done();
                await browser.close();
                throw new Error("Failed to load queens game");
            }
        }

        const board = await mainPage.evaluate(() => {
            const playingBoard = document.querySelector(".pr-game-web__main-container.pr-game-web__main-container--games .queens-board:not(.grid-board--disabled):not(.tutorial-board)");

            if(!playingBoard) {
                return null;
            }

            const colors: number[] = [];
            [...playingBoard.children[0].children].forEach(child => {
                [...child.classList].forEach(className => {
                    if(className.startsWith("cell-color-")) {
                        colors.push(Number(className.replace("cell-color-", "")) + 1);
                    }
                });
            });

            return colors;
        });

        LogBuilder
            .start()
            .level("INFO")
            .info("LinkedIn.GameSolver", "QueenSolver")
            .line("Got todays board.")
            .object("board", board)
            .done();

        const solverInput = board.join(",");

        try {
            const solved = await this.solver.solveQueens(solverInput);
            LogBuilder
                .start()
                .level("INFO")
                .info("LinkedIn.GameSolver", "QueenSolver")
                .line("Got solver result.")
                .object("solved", solved)
                .done();

            const gridSize = Math.sqrt(board.length);

            for(let y = 0; y < gridSize; y++) {
                for(let x = 0; x < gridSize; x++) {
                    if(solved.find(queen => queen.x === x && queen.y === y)) {
                        const cellSelector = `.pr-game-web__main-container.pr-game-web__main-container--games .queens-board:not(.grid-board--disabled)>div>div[data-cell-idx='${y * gridSize + x}']`;
                        try {
                            const cell = await mainPage.waitForSelector(cellSelector, {timeout: 5000});
                            await cell.click();
                            await cell.click();
                        }
                        catch {
                            LogBuilder
                                .start()
                                .level("ERROR")
                                .info("LinkedIn.GameSolver", "QueenSolver")
                                .line("Could not find cell to click.")
                                .object("cell", {x, y})
                                .object("selector", cellSelector)
                                .done();

                            continue;
                        }
                    }
                }
            }
        }
        catch {
            LogBuilder
                .start()
                .level("INFO")
                .info("LinkedIn.GameSolver", "QueenSolver")
                .line("Solver failed with input.")
                .object("solved", solverInput)
                .done();
        }
    }

    static sleep(ms: number): Promise<void> {
        return new Promise(res => setTimeout(res, ms));
    }
}
