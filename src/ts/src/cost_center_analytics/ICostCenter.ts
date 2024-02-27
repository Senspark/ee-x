import {ILibraryAnalytics, IPlugin} from "../core";

export interface ICostCenter extends IPlugin, ILibraryAnalytics {
    initialize(): void;
}