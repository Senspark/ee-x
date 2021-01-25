import { IAd } from "./IAd";

export enum FullScreenAdResult {
    Failed,
    Canceled,
    Completed,
}

export interface IFullScreenAd extends IAd {
    show(): Promise<FullScreenAdResult>;
}