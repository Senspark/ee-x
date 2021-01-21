export interface IFirebasePerformanceTrace {
    start(): void;
    stop(): void;
    putMetric(metricName: string, value: number): void;
    incrementMetric(metricName: string, incrementBy: number): void;
    getLongMetric(metricName: string): number;
}