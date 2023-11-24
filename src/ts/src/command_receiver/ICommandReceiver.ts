import {IObserverManager, IPlugin} from "../core";
import {ICmdData} from "./internal/CommandReceiver";

export interface ICommandReceiver extends IPlugin, IObserverManager<ICommandObserver> {
    initialize(): void;

    addCommand(data: ICmdData): void;
}

export interface ICommandObserver {
    onReceived(command: ICommand): void;
}

export interface ICommand {
    command: string;
    data: string;
}

