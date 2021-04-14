export interface ICapper {
    isCapped: boolean;
    cap(): void;
    lock(): void;
    unlock(): void;
}