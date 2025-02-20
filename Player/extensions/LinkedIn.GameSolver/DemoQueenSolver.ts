import puppeteer from "puppeteer";
import Solver from "./Solver";
import LogBuilder from "../../src/service/logger/LogBuilder";

export default class DemoQueenSolver {
    solver: Solver;

    constructor(solver: Solver) {
        this.solver = solver;
    }

    async solvePuzzle(): Promise<void> {
        const browser = await puppeteer.launch({
            headless: false,
            defaultViewport: null,
            args: [
                "--disable-gpu",
                "--disable-dev-shm-usage",
                "--disable-setuid-sandbox",
                "--no-first-run",
                "--no-sandbox",
                "--no-zygote",
                "--deterministic-fetch",
                "--disable-features=IsolateOrigins",
                "--disable-site-isolation-trials",
            ],
            targetFilter: t => t.url().includes("queens-linkedin.vercel.app") || t.url() === "about:blank",
        });

        const mainPage = (await browser.pages())[0];
        await mainPage.setBypassCSP(true);
        await mainPage.setBypassServiceWorker(true);

        await DemoQueenSolver.sleep(1000);

        await mainPage.goto("https://queens-linkedin.vercel.app/");
        await mainPage.bringToFront();

        const levels = await mainPage.evaluate(() => {
            return [...document.querySelectorAll(".grid>a>button")].filter(b => !b.attributes.getNamedItem("disabled")).map(b => Number(b.innerText));
        });

        for(const level of levels) {
            while(true) {
                try {
                    await mainPage.setOfflineMode(false);
                    await Promise.all([
                        mainPage.waitForNavigation(),
                        mainPage.goto(`https://queens-linkedin.vercel.app/level/${level}`),
                    ]);
                    await mainPage.bringToFront();
                    await mainPage.waitForSelector("div.board", {timeout: 1000});
                    await mainPage.setOfflineMode(true);
                    break;
                }
                catch {
                    // OIDA
                }
            }

            const parsed: number[] = await mainPage.evaluate(() => {
                const board = document.querySelector(".board");

                if(!board) {
                    return;
                }

                const colorIndexMapping: Record<string, number> = {};
                let index = 1;

                const parsedBrowser: number[] = [];

                [...board.children].forEach((c: HTMLDivElement) => {
                    if(!colorIndexMapping[c.style.backgroundColor]) {
                        colorIndexMapping[c.style.backgroundColor] = index;
                        index += 1;
                    }

                    parsedBrowser.push(colorIndexMapping[c.style.backgroundColor]);
                });

                return parsedBrowser;
            });

            const input = parsed.join(",");
            const solved = await this.solver.solveQueens(input);

            for(const queen of solved) {
                let errors = 0;
                while(true) {
                    try {
                        const cell1Selector = `div.board>[data-row='${queen.y}'][data-col='${queen.x}']`;
                        const cell2Selector = `div.board>[data-row='${queen.y}'][data-col='${queen.x}']>.cell-input--cross`;

                        await mainPage.waitForFunction((s) => {
                            return document.querySelector(s) !== null;
                        }, { polling: "mutation" }, cell1Selector);

                        await mainPage.click(cell1Selector);

                        await mainPage.waitForFunction((s) => {
                            return document.querySelector(s) !== null;
                        }, { polling: "mutation" }, cell2Selector);
                        await mainPage.click(cell2Selector);

                        break;
                    }
                    catch(err) {
                        LogBuilder
                            .start()
                            .level("ERROR")
                            .info("LinkedIn.GameSolver", "QueenSolver")
                            .line("Could not find cell to click.")
                            .object("cell", queen)
                            .object("error", err)
                            .object("frames", mainPage.frames())
                            .done();

                        errors++;

                        if(errors > 10) {
                            throw err;
                        }
                    }
                }
            }

            try {
                await mainPage.waitForSelector("div.game.relative div.flex a:last-child button", {timeout: 1000});
            }
            catch {
                // OIDA
            }
        }
    }

    static sleep(ms: number): Promise<void> {
        return new Promise(res => setTimeout(res, ms));
    }
}
