import {FlatTreeControl} from '@angular/cdk/tree';
import { Component } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import { interval, Observable } from 'rxjs';
import { startWith, switchMap } from "rxjs/operators";
import {MatTreeFlatDataSource, MatTreeFlattener} from '@angular/material/tree';

interface FoodNode {
  id: number;
  nombre: string;
  hijos?: FoodNode[];
}

const server = 'localhost';
const port = '8080';
const protocol = 'http';

const config = {
  prod: {
    base: `/`,
  },
  dev: {
    base: `${protocol}://${server}:${port}/`,
  }
}

interface ExampleFlatNode {
  expandable: boolean;
  nombre: string;
  id: number;
  level: number;
}

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.css'],
})
export class AppComponent {
  private _transformer = (node: FoodNode, level: number) => {
    return {
      expandable: !!node.hijos && node.hijos.length > 0,
      nombre: node.nombre,
      level: level,
    };
  }

  treeControl = new FlatTreeControl<ExampleFlatNode>(
      node => node.level, node => node.expandable);

  treeFlattener = new MatTreeFlattener(
      this._transformer, node => node.level, node => node.expandable, node => node.hijos);

  dataSource = new MatTreeFlatDataSource(this.treeControl, this.treeFlattener);

  

  urlProcs = config.prod.base + 'procs';
  urlCPU = config.prod.base + 'cpu';
  urlMEMO = config.prod.base + 'memo';
  urlProcsInfo = config.prod.base + 'procsinfo';
  urlKill = config.prod.base + 'killproc';
  urlArbol = config.prod.base + 'arbol';


  procesos: string[] = [];
  ejecucion: number = 0;
  suspendido: number = 0;
  detenido: number = 0;
  zombie: number = 0;
  otro: number = 0;
  total: number = 0;
  kill: number = 0;

  cpuUsed: number = 0;
  arbol: FoodNode[];

  cpuMemoTotalMb: number = 0;
  cpuMemoUsedMb: number = 0;

  constructor(private http: HttpClient) {
    
    interval(1000).pipe(
      startWith(0),
      switchMap(() => this.getProcs())
    ).subscribe(res => {
      this.suspendido = 0;
      this.procesos.length = 0;
      for (let key in res) {
        if (res.hasOwnProperty(key)) {
          this.procesos.push(res[key]);
        }
      }
      this.total = this.procesos.length;
    });

    interval(1500).pipe(
      startWith(0),
      switchMap(() => this.getCPU())
    ).subscribe(res => {
      this.cpuUsed = res['Used'];
    });

    interval(1500).pipe(
      startWith(0),
      switchMap(() => this.getArbol())
    ).subscribe(res => {
      this.arbol = res;
      this.dataSource.data = this.arbol;
    });

    interval(1000).pipe(
      startWith(0),
      switchMap(() => this.getMemo())
    ).subscribe(res => {
      this.cpuMemoTotalMb = res['TotalMb'];
      this.cpuMemoUsedMb = res['UsedMb'];
    });

    interval(1000).pipe(
      startWith(0),
      switchMap(() => this.getProcsInfo())
    ).subscribe(res => {
      for (let key in res) {
        if (res.hasOwnProperty(key)) {
          if (key === 'Running') {
            this.ejecucion = res[key];
          } else if (key === 'Sleeping') {
            this.suspendido = res[key];
          } else if (key === 'Stopped') {
            this.detenido = res[key];
          } else if (key === 'Zombie') {
            this.zombie = res[key];
          } else if (key === 'Other') {
            this.otro = res[key];
          } else if (key === 'Total') {
            this.total = res[key];
          }
        }
      }
    });
  }

  getArbol(): Observable<any> {
    return this.http.get(this.urlArbol);
  }

  getProcs(): Observable<any> {
    return this.http.get(this.urlProcs);
  }

  getCPU(): Observable<any> {
    return this.http.get(this.urlCPU);
  }

  getMemo(): Observable<any> {
    return this.http.get(this.urlMEMO);
  }

  getProcsInfo(): Observable<any> {
    return this.http.get(this.urlProcsInfo);
  }

  onClickKill(event): void {
    const pid: string = event.target.attributes.id.nodeValue;
    console.log(this.urlKill + `?pid=${pid}`);
    this.http.get(this.urlKill + `?pid=${pid}`).subscribe();
  }

  hasChild = (_: number, node: ExampleFlatNode) => node.expandable;
}
