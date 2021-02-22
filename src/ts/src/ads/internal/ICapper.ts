export interface ICapper {
    isCapped: boolean;
    cap(): void;
}