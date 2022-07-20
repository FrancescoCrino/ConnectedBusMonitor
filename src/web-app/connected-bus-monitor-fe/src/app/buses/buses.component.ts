import { Component, OnInit } from '@angular/core';

import { Bus } from '../bus';
import { BusMonitorService } from '../bus-monitor.service';
import {Observable} from "rxjs";

@Component({
  selector: 'app-buses',
  templateUrl: './buses.component.html',
  styleUrls: ['./buses.component.css']
})
export class BusesComponent implements OnInit {
  buses: string[] = [];


  constructor(private busMonitorService: BusMonitorService) { }

  ngOnInit(): void {
    this.getBuses();
  }

  getBuses(): void {
    this.busMonitorService.getBuses()
        .subscribe(buses => this.buses = buses);
  }


}
