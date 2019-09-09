<?php $__env->startSection('jsHead'); ?>
  <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
  <script type="text/javascript">
    var speeds = <?php echo $speeds; ?>;
    var dataTable = [] ;
    dataTable[0] = ['Id', 'RightSpeed', 'LeftSpeed'];
    for (index in speeds) {
      dataTable[parseInt(index)+1] = [speeds[index]['id'], speeds[index]['rightSpeed'], speeds[index]['leftSpeed']];
    }
    console.log(dataTable);
    google.charts.load('current', {'packages':['corechart']});
    google.charts.setOnLoadCallback(drawChart);

    function drawChart() {
      // var data = google.visualization.arrayToDataTable([
      //   ['Id', 'RightSpeed', 'LeftSpeed'],
      //   ['2013',  1000,      400],
      //   ['2014',  1170,      460],
      //   ['2015',  660,       1120],
      //   ['2016',  1030,      540]
      // ]);

      var data = google.visualization.arrayToDataTable(dataTable);

      var options = {
        title: 'Speed of motors',
        hAxis: {title: 'Count Id',  titleTextStyle: {color: '#333'}},
        vAxis: {minValue: 0}
      };

      var chart = new google.visualization.AreaChart(document.getElementById('chart_div'));
      chart.draw(data, options);
    }
  </script>
<?php $__env->stopSection(); ?>


<?php $__env->startSection('titleProgram',"Programa 2"); ?>



<?php $__env->startSection('content'); ?>
  <div  class="row d-flex justify-content-center" style="width: 100%; height: 500px;">
    <div id="chart_div" class="col">

    </div>
  </div>
  <div class="row">
    <div class="col">
      <div class=" d-flex justify-content-center">
        <form method="get" class="form-inline my-2 my-lg-0" action="<?php echo e(route('programa4.start')); ?>">
          <input class="form-control mr-sm-2" type="search" placeholder="SetPoint" aria-label="Search" name="setPoint">
          <button class="btn btn-outline-success my-2 my-sm-0" type="submit">START</button>
        </form>
      </div>
    </div>
    <div class="col">
      <div class=" d-flex justify-content-center"><a href="<?php echo e(route('programa4.stop')); ?>" class="btn btn-dark btn-lg">STOP</a></div>
    </div>
    <div class="col">
      <div class=" d-flex justify-content-center"><a href="<?php echo e(route('programa4.exit')); ?>" class="btn btn-dark btn-lg">BACK</a></div>
    </div>
  </div>
<?php $__env->stopSection(); ?>

<?php echo $__env->make('layouts.program', \Illuminate\Support\Arr::except(get_defined_vars(), ['__data', '__path']))->render(); ?><?php /**PATH /home/as/Documentos/EPSEM-UPC/TFG/FINAL_CODE/AP1/FINAL_PROJECT_WEB/SERVER_WEB/RobotCarKit/resources/views/programa4/chart.blade.php ENDPATH**/ ?>