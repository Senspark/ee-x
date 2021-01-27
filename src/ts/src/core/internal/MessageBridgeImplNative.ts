import { IMessageBridgeImpl } from "./IMessageBridgeImpl";

declare function ee_staticCall(tag: string, message: string): string;

export class MessageBridgeImplNative implements IMessageBridgeImpl {
    public call(tag: string, message: string): string {
        return ee_staticCall(tag, message);
    }
}