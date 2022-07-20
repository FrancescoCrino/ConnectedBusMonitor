import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';

import {Observable, of, pipe} from 'rxjs';
import { catchError, map, tap } from 'rxjs/operators';

//import { Hero } from './hero';
import { Bus } from './bus';
import { MessageService } from './message.service';
import {AirQuality} from "./air-quality";


@Injectable({ providedIn: 'root' })
export class BusMonitorService {

  //api/buses';
  private busMonitorUrl = 'https://w7zasr9b9i.execute-api.us-east-1.amazonaws.com/bus'; // URL to web api
  httpOptions = {
    headers: new HttpHeaders({ 'Content-Type': 'application/json' })
  };

  constructor(
    private http: HttpClient,
    private messageService: MessageService) { }

  /** GET buses from the server */
  /*
  getBuses(): Observable<Bus[]> {
    return this.http.get<Bus[]>(this.busMonitorUrl)
      .pipe(
        tap(_ => this.log('fetched buses')),
        catchError(this.handleError<Bus[]>('getBuses', []))
      );
  } */

  /** GET buses from the server */
  getBuses(): Observable<string[]> {
    return this.http.get<any>(this.busMonitorUrl)
        .pipe(
            map(data => {
              // this.log('response' + res.toString())
              let response = data.response;
              console.log('response', response)

              let buses = response.av_bus;
              // this.log('fetched buses' + buses)
              console.log('fetched buses', buses)

              return buses;
            }),
            catchError(this.handleError<any[]>('getBuses', []))
            );
  }

  /** GET bus by id. Return `undefined` when id not found */
  getBusNo404<Data>(id: number): Observable<Bus> {
    const url = `${this.busMonitorUrl}/?id=${id}`;
    return this.http.get<Bus[]>(url)
      .pipe(
        map(heroes => heroes[0]), // returns a {0|1} element array
        tap(h => {
          const outcome = h ? 'fetched' : 'did not find';
          this.log(`${outcome} hero id=${id}`);
        }),
        catchError(this.handleError<Bus>(`getHero id=${id}`))
      );
  }

  /** GET bus by id. Will 404 if id not found */
  getBus(id: number): Observable<Bus> {
    const url = `${this.busMonitorUrl}/${id}`;
    return this.http.get<any>(url).pipe(
        map(data => {
            let air_quality_records = data.response;

            let airQualityList = [];
            for (let air_quality_record of air_quality_records) {
                console.log(air_quality_record);
                let record_value = new AirQuality(air_quality_record.air_data.bus,
                                                    air_quality_record.date,
                                                    air_quality_record.air_data.temperature,
                                                    air_quality_record.air_data.humidity,
                                                    air_quality_record.air_data.co2,
                                                    air_quality_record.air_data.air_quality);
                airQualityList.push(record_value);

            }

            const sortedAirQualityList = airQualityList.sort(
                (objA, objB) => new Date(objA.date).getTime() - new Date(objB.date).getTime(),
            );

            let bus = new Bus(data.response[0].bus_id, sortedAirQualityList);

            console.log('bus', bus);
            return bus;
        }),
        catchError(this.handleError<Bus>(`getBus id=${id}`))
    );
  }

  /**
   * Handle Http operation that failed.
   * Let the app continue.
   *
   * @param operation - name of the operation that failed
   * @param result - optional value to return as the observable result
   */
  private handleError<T>(operation = 'operation', result?: T) {
    return (error: any): Observable<T> => {

      // TODO: send the error to remote logging infrastructure
      console.error(error); // log to console instead

      // TODO: better job of transforming error for user consumption
      this.log(`${operation} failed: ${error.message}`);

      // Let the app keep running by returning an empty result.
      return of(result as T);
    };
  }

  /** Log a BusMonitorService message with the MessageService */
  private log(message: string) {
    this.messageService.add(`BusMonitorService: ${message}`);
  }
}
