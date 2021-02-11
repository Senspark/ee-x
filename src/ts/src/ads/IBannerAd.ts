import { IAd } from "./IAd";

export interface IBannerAd extends IAd {
    isVisible: boolean;
    anchor: [number, number];
    position: [number, number];
    size: [number, number];
}