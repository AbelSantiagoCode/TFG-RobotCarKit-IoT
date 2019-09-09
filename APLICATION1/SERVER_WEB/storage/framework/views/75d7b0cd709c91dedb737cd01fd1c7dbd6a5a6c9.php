<?php $__env->startSection('styleHead'); ?>
  <link rel="stylesheet" href="/css/style.css">
<?php $__env->stopSection(); ?>
<?php $__env->startSection('jsHead'); ?>
  <script defer src="https://use.fontawesome.com/releases/v5.2.0/js/all.js" integrity="sha384-4oV5EgaV02iISL2ban6c/RmotsABqE4yZxZLcYMAdG7FAPsyHYAPpywE9PJo+Khy" crossorigin="anonymous"></script>
<?php $__env->stopSection(); ?>


<?php $__env->startSection('titleProgram',"Programa 1"); ?>



<?php $__env->startSection('content'); ?>
    <div class="row" style= "margin-top: 100px;">
      <div class="col"></div>
      <div class="col"></div>
      <div class="col p-5">
        <form method="get" class="form-inline my-2 my-lg-0" action="<?php echo e(route('programa1.forwardPID')); ?>">
          <div class="search-box">
            <input class="search-txt" type="text" name="forwardPID" value="" placeholder="Forwarder PID">
            <a href="" class="search-btn">
              <i class="fas fa-arrow-circle-up fa-2x"></i>
            </a>
          </div>
        </form>
      </div>
      <div class="col"></div>
      <div class="col"></div>
    </div>
    <div class="row">
      <div class="col"></div>
      <div class="col p-5">
        <div class="search-box">
          <a href="<?php echo e(route('programa1.leftRotation')); ?>" class="search-btn">
            <i class="fas fa-undo fa-2x"></i>
          </a>
        </div>
      </div>
      <div class="col p-5">
        <div class="search-box">
          <a href="<?php echo e(route('programa1.stop')); ?>" class="search-btn">
            <i class="far fa-stop-circle fa-2x"></i>
          </a>
        </div>
      </div>
      <div class="col p-5">
        <div class="search-box">
          <a href="<?php echo e(route('programa1.rightRotation')); ?>" class="search-btn">
            <i class="fas fa-redo fa-2x"></i>
          </a>
        </div>
      </div>
      <div class="col"></div>
    </div>
    <div class="row">
      <div class="col"></div>
      <div class="col"></div>
      <div class="col p-5">
        <form method="get" class="form-inline my-2 my-lg-0" action="<?php echo e(route('programa1.backwardPID')); ?>">
          <div class="search-box">
            <input class="search-txt" type="text" name="backwardPID" value="" placeholder="Backward PID">
            <a href="" class="search-btn">
              <i class="fas fa-arrow-circle-down fa-2x"></i>
            </a>
          </div>
        </form>
      </div>
      <div class="col"></div>
      <div class="col"></div>
    </div>

    <div class="row" style= "margin: 10px;">
      <div class="col"></div>
      <div class="col"></div>
      <div class="col p-5">
        <form method="get" class="form-inline my-2 my-lg-0" action="<?php echo e(route('programa1.powerRotation')); ?>">
          <div class="search-box">
            <input class="search-txt" type="text" name="powerRotation" value="" placeholder="Power Rotation">
            <a href="" class="search-btn">
              <i class="fas fa-charging-station fa-2x"></i>
            </a>
          </div>
        </form>
      </div>
      <div class="col"></div>
      <div class="col"></div>
    </div>


    <div class="row " style= "margin-top: 10px;">
      <div class="col p-5">
        <form method="get" class="form-inline my-2 my-lg-0" action="<?php echo e(route('programa1.moveCMForward')); ?>">
          <div class="search-box">
            <input class="search-txt" type="text" name="moveCMForward" value="" placeholder="Move Centimeter Forward">
            <a href="" class="search-btn">
              <i class="fas fa-angle-double-up fa-2x"></i>
            </a>
          </div>
        </form>
      </div>
      <div class="col p-5">
        <form method="get" class="form-inline my-2 my-lg-0" action="<?php echo e(route('programa1.moveCMBackward')); ?>">
          <div class="search-box">
            <input class="search-txt" type="text" name="moveCMBackward" value="" placeholder="Move Centimeter Backward">
            <a href="" class="search-btn">
              <i class="fas fa-angle-double-down fa-2x"></i>
            </a>
          </div>
        </form>
      </div>
      <div class="col p-5">
        <form method="get" class="form-inline my-2 my-lg-0" action="<?php echo e(route('programa1.leftCarRotation')); ?>">
          <div class="search-box">
            <input class="search-txt" type="text" name="leftCarRotation" value="" placeholder="Left Rotation Car">
            <a href="" class="search-btn">
              <i class="far fa-caret-square-left fa-2x"></i>
            </a>
          </div>
        </form>
      </div>
      <div class="col p-5">
        <form method="get" class="form-inline my-2 my-lg-0" action="<?php echo e(route('programa1.rightCarRotation')); ?>">
          <div class="search-box">
            <input class="search-txt" type="text" name="rightCarRotation" value="" placeholder="Right Rotation Car">
            <a href="" class="search-btn">
              <i class="far fa-caret-square-right fa-2x"></i>
            </a>
          </div>
        </form>
      </div>

      <div class="col p-5">
        <div class="search-box">
          <a href="<?php echo e(route('programa1.exit')); ?>" class="search-btn">
            <i class="fas fa-backspace "></i>
          </a>
        </div>
      </div>
    </div>
<?php $__env->stopSection(); ?>

<?php echo $__env->make('layouts.program', \Illuminate\Support\Arr::except(get_defined_vars(), ['__data', '__path']))->render(); ?><?php /**PATH /home/as/Documentos/EPSEM-UPC/TFG/FINAL_PROJECT_WEB/SERVER_WEB/RobotCarKit/resources/views/programa1/control.blade.php ENDPATH**/ ?>