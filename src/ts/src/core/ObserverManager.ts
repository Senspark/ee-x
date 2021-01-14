import { IObserverManager } from "./IObserverManager";

export class ObserverManager<Observer> implements IObserverManager<Observer> {
    private _counter = 0;
    private readonly _observers: { [index: number]: Observer } = {};

    public addObserver(observer: Observer): number {
        const id = this._counter++;
        this._observers[id] = observer;
        return id;
    }

    public removeObserver(id: number): boolean {
        if (this._observers[id]) {
            delete this._observers[id];
            return true;
        }
        return false;
    }

    public dispatchEvent(dispatcher: (observer: Observer) => void): void {
        for (const key in this._observers) {
            const observer = this._observers[key];
            dispatcher(observer);
        }
    }
}