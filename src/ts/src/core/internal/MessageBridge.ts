import {
    IMessageBridge,
    MessageHandler,
} from '../IMessageBridge';
import { IMessageBridgeImpl } from './IMessageBridgeImpl';
import { MessageBridgeImplNative } from './MessageBridgeImplNative';

export class MessageBridge implements IMessageBridge {
    private _callbackCounter = 0;
    private readonly _handlers: { [index: string]: MessageHandler } = {};
    private readonly _impl: IMessageBridgeImpl;

    public constructor() {
        this._impl = new MessageBridgeImplNative(this.callCpp);
    }

    public registerHandler(handler: MessageHandler, tag: string): void {
        if (this._handlers[tag]) {
            throw new Error(`Failed to register handler: ${tag}`);
        }
        this._handlers[tag] = handler;
    }

    public deregisterHandler(tag: string): void {
        if (!this._handlers[tag]) {
            throw new Error(`Failed to deregister handler: ${tag}`);
        }
        delete this._handlers[tag];
    }

    private findHandler(tag: string): MessageHandler | undefined {
        return this._handlers[tag];
    }

    public call(tag: string, message?: string): string {
        return this._impl.call(tag, message ?? ``);
    }

    public callAsync(tag: string, message?: string): Promise<string> {
        return new Promise<string>(resolver => {
            const callbackTag = `${tag}${this._callbackCounter++}`;
            this.registerHandler(callbackMessage => {
                this.deregisterHandler(callbackTag);
                resolver(callbackMessage);
            }, callbackTag);
            const request = {
                [`callback_tag`]: callbackTag,
                [`message`]: message ?? ``,
            };
            this.call(tag, JSON.stringify(request));
        });
    }

    public callCpp(tag: string, message: string): void {
        const handler = this.findHandler(tag);
        if (handler === undefined) {
            throw new Error(`Failed to call handler: ${tag}`);
        }
        handler(message);
    }
}