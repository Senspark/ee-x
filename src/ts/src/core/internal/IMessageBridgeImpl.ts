export interface IMessageBridgeImpl {
    call(tag: string, message: string): string;
}
