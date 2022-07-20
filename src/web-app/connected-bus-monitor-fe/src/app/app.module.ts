import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';

import { AppComponent } from './app.component';
import { FormsModule } from "@angular/forms";
import { HttpClientModule } from "@angular/common/http";
// import { BusSearchComponent } from './bus-search/bus-search.component';
import { BusesComponent } from './buses/buses.component';
import { BusDetailComponent } from './bus-detail/bus-detail.component';
import { AppRoutingModule } from './app-routing.module';
import { GraphicalMaterialModule } from './graphical-material.module';
import { NgChartsModule } from 'ng2-charts';
import {CustomChartComponent} from './custom-chart/custom-chart.component';
import {DatePipe} from "@angular/common";

@NgModule({
  declarations: [
    AppComponent,
  //  BusSearchComponent,
    CustomChartComponent,
    BusesComponent,
    BusDetailComponent
  ],
  imports: [
    BrowserModule,
    FormsModule,
    GraphicalMaterialModule,
    NgChartsModule,
    AppRoutingModule,
    HttpClientModule
  ],
  providers: [DatePipe],
  bootstrap: [AppComponent]
})
export class AppModule { }
