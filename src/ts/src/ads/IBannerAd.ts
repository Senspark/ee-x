import { IAd } from "./IAd";

export interface IBannerAd extends IAd {
    anchor: [number, number];
    position: [number, number];
    size: [number, number];
    isVisible: boolean;
}