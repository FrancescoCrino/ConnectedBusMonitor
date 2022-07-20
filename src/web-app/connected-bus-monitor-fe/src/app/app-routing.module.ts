import { NgModule } from '@angular/core';
import { RouterModule, Routes } from '@angular/router';

// import { DashboardComponent } from './dashboard/dashboard.component';
import { BusesComponent } from './buses/buses.component';
import { BusDetailComponent } from './bus-detail/bus-detail.component';

const routes: Routes = [
    { path: '', redirectTo: '/buses', pathMatch: 'full' },
//    { path: 'dashboard', component: DashboardComponent },
    { path: 'detail/:id', component: BusDetailComponent },
    { path: 'buses', component: BusesComponent }
];

@NgModule({
    imports: [ RouterModule.forRoot(routes) ],
    exports: [ RouterModule ]
})
export class AppRoutingModule {}