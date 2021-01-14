export type MessageHandler = (message: string) => void;

export interface IMessageBridge {
    registerHandler(handler: MessageHandler, tag: string): void;
    deregisterHandler(tag: string): void;
    call(tag: string, message?: string): string;
    callAsync(tag: string, message?: string): Promise<string>;
}