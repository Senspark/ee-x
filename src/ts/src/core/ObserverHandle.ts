import { IObserverManager } from "./IObserverManager";
import { ObserverBinder } from "./ObserverBinder";
import { ScopeGuard } from "./ScopeGuard";

export class ObserverHandle {
    private readonly _guards: ScopeGuard[] = [];

    public bind<Observer>(manager: IObserverManager<Observer>): ObserverBinder<Observer> {
        return new ObserverBinder<Observer>(this, manager);
    }

    public add(rollback: () => void) {
        this._guards.push(new ScopeGuard(rollback));
    }

    public clear(): void {
        for (const entry of this._guards) {
            entry.dispose();
        }
        this._guards.length = 0;
    }
}