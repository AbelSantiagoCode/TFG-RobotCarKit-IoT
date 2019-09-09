@extends('layouts.program')

@section('jsHead')
  <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
  <script type="text/javascript">

    var positions = {!! $positions !!};
    var dataTable = [] ;
    dataTable[0] = ['Centimeters X', 'Centimeters Y'];
    for (index in positions) {
      dataTable[parseInt(index)+1] = [positions[index]['coordenateX'], positions[index]['coordenateY']];
    }
    console.log(dataTable);

    google.charts.load('current', {'packages':['corechart']});
    google.charts.setOnLoadCallback(drawChart);

    function drawChart() {
      // var data = google.visualization.arrayToDataTable([
      //   ['Centimeters X', 'Centimeters Y'],
      //   [0,0],
      //   [1,1],
      //   [2,2],
      //   [3,3],
      //   [-1,10],
      //   [-2,5],
      //   [-3,-3],
      //   [-5,5]
      //
      // ]);
      var data = google.visualization.arrayToDataTable(dataTable);

      var options = {
        title: 'Trajectory of Car',
        hAxis: {title: 'Centimeters X', minValue: -15, maxValue: 15},
        vAxis: {title: 'Centimeters Y', minValue: -15, maxValue: 15},
        colors: ['#795548'],
        pointSize: 10,
        pointShape: 'square'


      };

      //var chart = new google.visualization.AreaChart(document.getElementById('chart_div'));
      //var chart = new google.visualization.ScatterChart(document.getElementById('chart_div'));
      var chart = new google.visualization.LineChart(document.getElementById('chart_div'));


      chart.draw(data, options);
    }
  </script>
@endsection


@section('titleProgram',"Programa 3")



@section('content')
  <div  class="row d-flex justify-content-center" style="width: 100%; height: 500px;">
    <div id="chart_div" class="col"></div>
  </div>

  <div class="row d-flex justify-content-center">
    <div class="col">
      <div class=" d-flex justify-content-center"><a href="{{route('programa3.start')}}" class="btn btn-dark btn-lg">START</a></div>
    </div>
    <div class="col">
      <div class=" d-flex justify-content-center"><a href="{{route('programa3.stop')}}" class="btn btn-dark btn-lg">STOP</a></div>
    </div>
    <div class="col">
      <div class=" d-flex justify-content-center"><a href="{{route('programa3.exit')}}" class="btn btn-dark btn-lg">BACK</a></div>
    </div>
  </div>
@endsection
