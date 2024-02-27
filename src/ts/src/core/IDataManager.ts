export interface IDataManager {
    getValue<T>(key: string, defaultValue: T): T;

    setValue<T>(key: string, value: T): void;
}