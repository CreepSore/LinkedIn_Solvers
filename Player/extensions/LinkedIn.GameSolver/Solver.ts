import * as childProcess from "child_process";
import LogBuilder from "../../src/service/logger/LogBuilder";

export default class Solver {
    private path: string;

    constructor(solverPath: string) {
        this.path = solverPath;
    }

    @LogBuilder.$logRuntimePromise("none", ["LinkedIn.GameSolver", "Solver"])
    solveQueens(input: string): Promise<{x: number, y: number}[]> {
        return new Promise((res, rej) => {
            childProcess.exec(`${this.path} -queens "${input}"`, (err, stdout, stderr) => {
                if(err) {
                    rej(err);
                    return;
                }

                const result = stdout.trim().split("\n").map(line => {
                    const [x, y] = line.split("x").map(num => parseInt(num, 10));
                    return {x, y};
                });

                res(result);
            });
        });
    }
}
