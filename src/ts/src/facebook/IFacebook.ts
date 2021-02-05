import { IPlugin } from "../core";
import { FacebookGraphRequest } from "./FacebookGraphRequest";
import { IFacebookAccessToken } from "./IFacebookAccessToken";

export enum FacebookShareType {
    Link,
    Photo,
    Video,
}

export interface FacebookLoginResult {
    successful: boolean;
    canceled: boolean;
    errorMessage: string;
}

export interface FacebookGraphResult {
    successful: boolean;
    response: string;
    errorMessage: string;
}

export interface FacebookShareResult {
    successful: boolean;
    canceled: boolean;
    errorMessage: string;
}

export interface IFacebook extends IPlugin {
    isLoggedIn: boolean;
    logIn(permissions: string[]): Promise<FacebookLoginResult>;
    logOut(): Promise<void>;
    accessToken?: IFacebookAccessToken;
    graphRequest(request: FacebookGraphRequest): Promise<FacebookGraphResult>;
    shareContent(type: FacebookShareType, url: string): Promise<FacebookShareResult>;
}