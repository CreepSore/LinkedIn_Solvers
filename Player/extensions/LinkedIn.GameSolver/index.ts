import {EventEmitter} from "events";
import * as path from "path";

import IExecutionContext, { IAppExecutionContext, ICliExecutionContext } from "@service/extensions/IExecutionContext";
import IExtension, { ExtensionMetadata } from "@service/extensions/IExtension";
import ConfigLoader from "@logic/config/ConfigLoader";
import Core from "@extensions/Core";
import Solver from "./Solver";
import LogBuilder from "../../src/service/logger/LogBuilder";
import QueenSolver from "./QueenSolver";
import DemoQueenSolver from "./DemoQueenSolver";

class LinkedInGameSolverConfig {
    solverPath: string = "../Solver/out/build/x64-Release/Queen-Solver.exe";
}

export default class LinkedInGameSolver implements IExtension {
    static metadata: ExtensionMetadata = {
        name: "LinkedIn.GameSolver",
        version: "1.0.0",
        description: "Template Module",
        author: "ehdes",
        dependencies: [Core],
    };

    metadata: ExtensionMetadata = LinkedInGameSolver.metadata;

    config: LinkedInGameSolverConfig = new LinkedInGameSolverConfig();
    events: EventEmitter = new EventEmitter();
    $: <T extends IExtension>(name: string|Function & { prototype: T }) => T;

    constructor() {
        this.config = this.loadConfig(true);
    }

    async start(executionContext: IExecutionContext): Promise<void> {
        this.checkConfig();
        this.$ = <T extends IExtension>(name: string|Function & { prototype: T }) => executionContext.extensionService.getExtension(name) as T;
        if(executionContext.contextType === "cli") {
            await this.startCli(executionContext);
            return;
        }
        else if(executionContext.contextType === "app") {
            await this.startMain(executionContext);
            return;
        }
    }

    async stop(): Promise<void> {

    }

    private async startCli(executionContext: ICliExecutionContext): Promise<void> {

    }

    private async startMain(executionContext: IAppExecutionContext): Promise<void> {
        const solver = new Solver(path.resolve(this.config.solverPath));
        // const queenSolver = new QueenSolver(solver);
        // await queenSolver.solvePuzzle();

        const demoQueenSolver = new DemoQueenSolver(solver);
        await demoQueenSolver.solvePuzzle();

        /*
        solver.solveQueens("1,1,1,1,1,1,1,1,1,1,4,4,2,2,2,1,6,6,6,4,2,3,1,1,6,7,6,4,4,3,1,1,6,6,6,5,5,3,1,1,6,8,8,8,5,3,3,3,8,8,8,8,5,5,8,8,8,8,8,8,8,8,8,8")
            .then(result => {
                LogBuilder
                    .start()
                    .level("INFO")
                    .info("LinkedIn.GameSolver", "Queens")
                    .object("result", result)
                    .line("Got result.")
                    .done();
            });
        */
    }

    private checkConfig(): void {
        if(!this.config) {
            throw new Error(`Config could not be found at [${this.generateConfigNames()[0]}]`);
        }
    }

    private loadConfig(createDefault: boolean = false): typeof this.config {
        const [configPath, templatePath] = this.generateConfigNames();
        return ConfigLoader.initConfigWithModel(
            configPath,
            templatePath,
            new LinkedInGameSolverConfig(),
            createDefault,
        );
    }

    private generateConfigNames(): string[] {
        return [
            ConfigLoader.createConfigPath(`${this.metadata.name}.json`),
            ConfigLoader.createTemplateConfigPath(`${this.metadata.name}.json`),
        ];
    }
}
