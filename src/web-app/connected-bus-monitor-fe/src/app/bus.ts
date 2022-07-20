import {AirQuality} from "./air-quality";

export class Bus {

  constructor(
  public id: number,
  public air_data: AirQuality[]
//  name: string;
  ) { }
}