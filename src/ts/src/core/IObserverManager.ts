export interface IObserverManager<Observer> {
    addObserver(observer: Observer): number;
    removeObserver(id: number): boolean;
    dispatchEvent(dispatcher: (observer: Observer) => void): void;
}