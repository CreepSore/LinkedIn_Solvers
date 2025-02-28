// ==UserScript==
// @name        queens-linkedin.vercel.app Solver
// @namespace   Violentmonkey Scripts
// @match       https://queens-linkedin.vercel.app/*
// @grant       GM_xmlhttpRequest
// @grant       GM_getValue
// @grant       GM_setValue
// @version     1.0
// @author      -
// @description 21.2.2025, 18:53:45
// ==/UserScript==

(async() => {
    const endpointUrl = "https://beta.ehdes.com/linkedin.queens/solve";

    const getCurrentPuzzle = () => {
        if(!location.href.includes("level")) {
            return -1;
        }

        return Number(location.href.split("/").pop());
    };

    const getNextPuzzle = (fromLevel = -1) => {
        const _fromLevel = fromLevel === -1
            ? getCurrentPuzzle()
            : fromLevel;

        const levels = loadLevels();
        return levels.filter(a => Number(a) !== _fromLevel && _fromLevel < Number(a)).sort((a, b) => {
            return Math.abs(a - _fromLevel) - Math.abs(b - _fromLevel);
        })[0];
    };

    const getLevelsAnchorElements = () => {
        return [...document.querySelectorAll('.grid>a')].filter(a => a.children[0].getAttribute("disabled") === null);
    };

    const getBoardElement = () => {
        return document.querySelector("div.board");
    };

    const waitFor = (callback) => {
        return new Promise(res => {
            const interval = setInterval(() => {
                if(callback()) {
                    clearInterval(interval);
                    res();
                }
            }, 100);
        });
    };

    const waitForBoardElement = async() => {
        return waitFor(() => !!getBoardElement());
    }

    const loadLevels = () => {
        return GM_getValue('levels');
    };

    const saveLevels = (levels) => {
        GM_setValue("levels", levels);
    };

    const boardToSolverInput = () => {
        const board = getBoardElement();
        if(!board) {
            return;
        }

        const colors = {};
        /** @type {HTMLDivElement[]} */
        const children = [...board.children];
        let colorIndex = 1;
        let result = "";
        for(let i = 0; i < children.length; i++) {
            const child = children[i];

            if(!colors[child.style.backgroundColor]) {
                colors[child.style.backgroundColor] = colorIndex;
                colorIndex++;
            }

            if(i !== 0) {
                result += ",";
            }

            result += String(colors[child.style.backgroundColor]);
        }

        return result;
    };

    const handleLevelsPage = async() => {
        if(location.href.includes("level")) {
            return;
        }

        await waitFor(() => !!document.querySelector(".grid"));

        const levels = getLevelsAnchorElements();
        if(levels.length === 0) {
            return;
        }

        saveLevels(levels.map(a => a.href.split("/").pop()));
    };

    const handlePuzzlePage = async() => {
        if(!location.href.includes("level")) {
            return;
        }

        await waitForBoardElement();
        const solverInput = boardToSolverInput();
        const solution = await getSolution(solverInput);

        for(const step of solution) {
            await clickCoordinate(step.x, step.y);
            await clickCoordinate(step.x, step.y);
        }

        await waitFor(() => document.querySelector(".game>.absolute"));

        const nextPuzzle = getNextPuzzle();

        location.href = location.href.split("/").slice(0, -1).join("/") + "/" + nextPuzzle;
    };

    const clickCoordinate = (x, y) => {
        return new Promise(res => {
            const element = document.querySelector(`div[data-row='${y}'][data-col='${x}']`);
            if(!element) {
                return;
            }

            element.dispatchEvent(new TouchEvent("pointerdown", {
                bubbles: true,
                cancelable: true,
                // @ts-ignore
                buttons: 1,
            }));

            setTimeout(() => {
                element.dispatchEvent(new TouchEvent("pointerup", {
                    bubbles: true,
                    cancelable: true,
                    // @ts-ignore
                    buttons: 1,
                }));

                res();
            }, 2);
        });
    }

    /**
     * @returns {Promise<{x: number, y: number}[]>}
     */
    const getSolution = (input) => {
        return new Promise((res, rej) => {
            GM_xmlhttpRequest({
                url: `${endpointUrl}?input=${encodeURIComponent(input)}`,
                responseType: "json",
                onload: (response) => {
                    const json = response.response;
                    res(json);
                },
                onerror: () => {
                    location.reload();
                    rej();
                }
            });
        });
    };

    const init = async() => {
        await handleLevelsPage();
        await handlePuzzlePage();
    };

    await init();
})();