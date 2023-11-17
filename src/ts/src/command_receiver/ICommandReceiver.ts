import {IObserverManager, IPlugin} from "../core";

export interface ICommandReceiver extends IPlugin, IObserverManager<ICommandObserver> {
    initialize(): void;
}

export interface ICommandObserver {
    onReceived(command: ICommand): void;
}

export interface ICommand{
    command: string;
    data: string;
}

