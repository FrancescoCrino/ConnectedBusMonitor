 import { MatCardModule } from "@angular/material/card";
 import { MatTableModule } from "@angular/material/table";
 import { NgModule } from "@angular/core";

@NgModule({
    imports: [MatCardModule, MatTableModule],
    exports: [MatCardModule, MatTableModule],
})
export class GraphicalMaterialModule { }
