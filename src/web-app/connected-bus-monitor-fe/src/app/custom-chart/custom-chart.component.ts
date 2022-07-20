import {Component, Input, OnInit} from '@angular/core';
import { ChartDataset, ChartOptions } from 'chart.js';
import {AirQuality} from "../air-quality";
import { DatePipe } from '@angular/common';


@Component({
    selector: 'custom-chart',
    templateUrl: './custom-chart.component.html',
    styleUrls: ['./custom-chart.component.css']
})
export class CustomChartComponent implements OnInit {

    @Input() chartData: AirQuality[] = [];
    @Input() metric: string = '';

    public lineChartData: ChartDataset[] = [
        { type: 'line', data: [] },
    ];
    public lineChartLabels: string[] = [];

    public lineChartColors = [
        {
            borderColor: "#7DB3FF",
            backgroundColor: "rgba(106, 110, 229, .16)"
        }
    ];

    public chartColors: Array<any> = [
        {
            backgroundColor: ['rgba(63, 191, 127,  0.4)','rgba(191, 191, 63, 0.4)'],

            borderColor: ['rgba(63, 191, 127, 0.8)', 'rgba(63, 191, 191, 0.8)'],
            hoverBackgroundColor: ['rgba(63, 191, 127, 0.6)', 'rgba(63, 191, 191, 0.6)'],
            borderWidth: 2
        }];

    public chartOptions: any = {
        responsive: true,
        maintainAspectRatio: false,
        title: {
            text: 'Temperature',
            display: true
            }
    };
    public lineChartLegend = false;
    public lineChartType = 'line';
    public lineChartPlugins = [];

    constructor(private datePipe: DatePipe) { }

    ngOnInit() {
        if (this.metric == 'temperature') {
            this.lineChartData[0].data = this.chartData.map(record => record.temperature);
        }
        else if (this.metric == 'humidity') {
            this.lineChartData[0].data = this.chartData.map(record => record.humidity);
        }
        else if (this.metric == 'co2') {
            this.lineChartData[0].data = this.chartData.map(record => record.co2);
        }
        this.lineChartLabels = this.chartData.map(record => this.transformDate(record.date) || '');
    }

    transformDate(date: string) {
        return this.datePipe.transform(date, 'dd/MM/yyyy HH:mm:ss');
    }
}
