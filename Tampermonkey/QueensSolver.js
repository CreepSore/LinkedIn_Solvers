// ==UserScript==
// @name     Queens Solver
// @version  1
// @grant    GM_xmlhttpRequest
// @match    https://www.linkedin.com/*
// ==/UserScript==

(async() => {
  const endpointUrl = "https://localhost/linkedin.queens/solve";

  let width = 0;

  const getStartButtonElement = () => {
    return document.querySelector(".launch-footer__btn--start");
  }

  const getPreloadBoardElement = () => {
    return document.querySelector(".pr-game-web__main-container.pr-game-web__main-container--games .queens-board.grid-board--disabled:not(.tutorial-board)");
  }

  const getBoardElement = () => {
    return document.querySelector(".pr-game-web__main-container.pr-game-web__main-container--games .queens-board:not(.grid-board--disabled):not(.tutorial-board)");
  };

  const waitForPreloadBoard = () => {
    return new Promise(res => {
      let interval = setInterval(() => {
        if(getPreloadBoardElement()) {
          res();
        }
      }, 10);
    });
  };

  const waitForBoard = () => {
    return new Promise(res => {
      let interval = setInterval(() => {
        if(getBoardElement()) {
          res();
        }
      }, 10);
    });
  };

  const getPreloadBoard = () => {
    const playingBoard = getPreloadBoardElement();

    if(!playingBoard) {
      return null;
    }

    const colors = [];
    [...playingBoard.children[0].children].forEach(child => {
      [...child.classList].forEach(className => {
        if(className.startsWith("cell-color-")) {
          colors.push(Number(className.replace("cell-color-", "")) + 1);
        }
      });
    });

    width = Math.sqrt(colors.length);

    return colors;
  };

  const getBoard = () => {
    const playingBoard = getBoardElement();

    if(!playingBoard) {
      return null;
    }

    const colors = [];
    [...playingBoard.children[0].children].forEach(child => {
      [...child.classList].forEach(className => {
        if(className.startsWith("cell-color-")) {
          colors.push(Number(className.replace("cell-color-", "")) + 1);
        }
      });
    });

    width = Math.sqrt(colors.length);

    return colors;
  };

  const clickCoordinate = (x, y) => {
    const idx = (y * width) + x
    const element = document.querySelector(`.queens-cell-with-border[data-cell-idx='${idx}']`);

    element.dispatchEvent(new MouseEvent("mousedown", {
        bubbles: true,
        cancelable: true,
        // @ts-ignore
        buttons: 1,
    }));

    element.dispatchEvent(new MouseEvent("mouseup", {
        bubbles: true,
        cancelable: true,
        // @ts-ignore
        buttons: 1,
    }));
  };

  const fetchSolution = (input) => {
    return new Promise(res => {
      GM_xmlhttpRequest({
        url: `${endpointUrl}?input=${encodeURIComponent(input)}`,
        responseType: "json",
        onload: (response) => {
          const json = response.response;
          res(json);
          debugger;
        },
        onerror: () => {
          location.reload();
        }
      });
    });
  }

  await Promise.any([
    /*
    (async() => {
      console.log("Waiting for PreloadBoard");
      await waitForPreloadBoard();

      let input = getPreloadBoard().join();
      console.log("Board: ", input);

      try {
        let solution = await fetchSolution(input);
        console.log("Solution: ", solution);

        getStartButtonElement().children[0].innerText += " (solution preloaded)";

        await waitForBoard();
        for(let i = 0; i < solution.length; i++) {
          const currentSolution = solution[i];

          clickCoordinate(currentSolution.x, currentSolution.y);
          clickCoordinate(currentSolution.x, currentSolution.y);
        }
      }
      catch {
        getStartButtonElement().children[0].innerText += " (solution preload failed)";
      }
    })(),
    */
    (async() => {
      console.log("Waiting for Board");
      await waitForBoard();

      let input = getBoard().join();
      console.log("Board: ", input);

      let solution = await fetchSolution(input);
      console.log("Solution: ", solution);

      for(let i = 0; i < solution.length; i++) {
        const currentSolution = solution[i];

        clickCoordinate(currentSolution.x, currentSolution.y);
        clickCoordinate(currentSolution.x, currentSolution.y);
      }
    })()
  ]);

})();
