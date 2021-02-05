import {
    ILogger,
    IMessageBridge,
    Utils,
} from "../../core";
import { FacebookGraphRequest } from "../FacebookGraphRequest";
import { FacebookGraphResult, FacebookLoginResult, FacebookShareResult, FacebookShareType, IFacebook } from "../IFacebook";
import { IFacebookAccessToken } from "../IFacebookAccessToken";

type Destroyer = () => void;

export class Facebook implements IFacebook {
    private readonly kPrefix = `FacebookBridge`;
    private readonly kRegisterNotifications = `${this.kPrefix}RegisterNotifications`;
    private readonly kIsLoggedIn = `${this.kPrefix}IsLoggedIn`;
    private readonly kLogIn = `${this.kPrefix}LogIn`;
    private readonly kLogOut = `${this.kPrefix}LogOut`;
    private readonly kGetAccessToken = `${this.kPrefix}GetAccessToken`;
    private readonly kOnProfileChanged = `${this.kPrefix}OnProfileChanged`;
    private readonly kGraphRequest = `${this.kPrefix}GraphRequest`;
    private readonly kSendRequest = `${this.kPrefix}SendRequest`;
    private readonly kShareContent = `${this.kPrefix}ShareContent`;

    private readonly _bridge: IMessageBridge;
    private readonly _logger: ILogger;
    private readonly _destroyer: Destroyer;

    public constructor(bridge: IMessageBridge, logger: ILogger, destroyer: Destroyer) {
        this._bridge = bridge;
        this._logger = logger;
        this._destroyer = destroyer;
    }

    public destroy(): void {
        this._destroyer();
    }

    public get isLoggedIn(): boolean {
        const response = this._bridge.call(this.kIsLoggedIn);
        return Utils.toBool(response);
    }

    public async logIn(permissions: string[]): Promise<FacebookLoginResult> {
        const request = {
            permissions: permissions,
        };
        const response = await this._bridge.callAsync(this.kLogIn, JSON.stringify(request));
        const json: FacebookLoginResult = JSON.parse(response);
        return json;
    }

    public async logOut(): Promise<void> {
        await this._bridge.callAsync(this.kLogOut);
    }

    public get accessToken(): IFacebookAccessToken | undefined {
        const response = this._bridge.call(this.kGetAccessToken);
        if (response.length === 0) {
            return undefined;
        }
        const json: IFacebookAccessToken = JSON.parse(response);
        return json;
    }

    public async graphRequest(request: FacebookGraphRequest): Promise<FacebookGraphResult> {
        const response = await this._bridge.callAsync(this.kGraphRequest, request.toString());
        const json: FacebookGraphResult = JSON.parse(response);
        return json;
    }

    public async shareContent(type: FacebookShareType, url: string): Promise<FacebookShareResult> {
        const request = {
            type: type,
            url: url,
        };
        const response = await this._bridge.callAsync(this.kShareContent, JSON.stringify(request));
        const json: FacebookShareResult = JSON.parse(response);
        return json;
    }
}