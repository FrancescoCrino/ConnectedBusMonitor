import { Component, OnInit } from '@angular/core';
import { ActivatedRoute } from '@angular/router';
import { Location } from '@angular/common';
import { Bus } from '../bus';
import { BusMonitorService } from '../bus-monitor.service';

@Component({
  selector: 'bus-detail',
  templateUrl: './bus-detail.component.html',
  styleUrls: ['./bus-detail.component.css']
})
export class BusDetailComponent implements OnInit {
  bus: Bus | undefined;
  displayedColumns: string[] = ['id', 'date', 'temperature', 'humidity', 'co2', 'air_quality'];

  dataSource: any[] = [
      {label: 'Ideal', class: 'text-success', temperature: '20 - 25', humidity: '30 - 60', co2: '< 800'},
      {label: 'Acceptable', class: 'text-warning', temperature: '25-31', humidity: '60-70', co2: '800 - 1100'},
      {label: 'Critical', class: 'text-error', temperature: '< 20 | > 31', humidity: '< 30 | > 70', co2: '> 1100'}];

  displayedCols: string[] = ['label', 'temperature', 'humidity', 'co2'];

  constructor(
      private route: ActivatedRoute,
      private busMonitorService: BusMonitorService,
      private location: Location
  ) {
  }

  ngOnInit(): void {
    this.getBus();
  }

  goBack(): void {
    this.location.back();
  }

  getBus(): void {
    const id = parseInt(this.route.snapshot.paramMap.get('id')!, 10);
    console.log(id);
    this.busMonitorService.getBus(id)
        .subscribe(bus => this.bus = bus);
  }

  getState(key: string, value: number): string {

    if (key == 'humidity') {
      if (value > 30 && value <= 60) {
        return 'text-success';
      }
      else if (value > 60 && value < 70) {
        return 'text-warning';
      }
      else {
        return 'text-error';
      }
    }
    else if (key == 'co2') {
      return value < 800 ? 'text-success' : value > 1100 ? 'text-error' : 'text-warning';
    }
    else if (key == 'temperature') {
      return value >= 20 && value <= 25 ? 'text-success' : value > 25 && value <= 31 ? 'text-warning' : 'text-error';

    }
    else {
      return '';
    }

  }



}