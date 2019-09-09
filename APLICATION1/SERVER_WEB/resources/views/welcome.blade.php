<!DOCTYPE html>
<html lang="en" dir="ltr">
  <head>
    <title>RobotCarKit</title>

    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <meta http-equiv="x-ua-compatible" content="ie-edge">

    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
  </head>
  <body >
    <!-- *************************************************NOTE: Empieza el contenido************************************************* -->
    <nav class="navbar navbar-expand-lg navbar-dark bg-dark d-flex fixed-top">
      <a href="" class="" style="width:150px">
        <img src="{{asset('img/IoT.png')}}" alt="Responsive image" style= "max-width: 70%; height: auto;" class="img-fluid">
      </a>

      <a class="navbar-brand" href="#">
        Robot Car Kit: adaptació a IoT
      </a>


      <button class="navbar-toggler" type="button" data-toggle="collapse" data-target="#navbarNavAltMarkup" aria-controls="navbarNavAltMarkup" aria-expanded="false" aria-label="Toggle navigation">
        <span class="navbar-toggler-icon"></span>
      </button>


      <div class="collapse navbar-collapse justify-content-end" id="navbarNavAltMarkup">
        <div class="navbar-nav">
          <a class="nav-item nav-link active" href="{{ route('programa1')}}">Programa1</a>
          {{-- <a class="nav-item nav-link" href="#">Programa2</a> --}}
          <a class="nav-item nav-link" href="{{ route('programa4')}}" >Programa2</a>
          <a class="nav-item nav-link" href="{{ route('programa3')}}" >Programa3</a>
        </div>
      </div>
    </nav> <!-- NOTE: END NAV -->


    <div class="d-flex justify-content-center" style= "width: 100%;margin-top: 100px;">
  <div id="carouselExampleCaptions" class="carousel slide " data-ride="carousel" style="width: 50%; height:30%;">
    <ol class="carousel-indicators">
      <li data-target="#carouselExampleCaptions" data-slide-to="0" class="active"></li>
      <li data-target="#carouselExampleCaptions" data-slide-to="1"></li>
      <li data-target="#carouselExampleCaptions" data-slide-to="2"></li>
    </ol>
    <div class="carousel-inner h-50">
      <div class="carousel-item active">
        <img src="{{asset('img/elegoo.jpg')}}" class="d-block w-100" alt="...">
        <div class="carousel-caption d-none d-md-block">
          <h5 style="color: black">ROBOT CAR KIT DE ELEGOO</h5>

        </div>
      </div>
      <div class="carousel-item">
        <img src="{{asset('img/blob_fXIeojA4b3.blob.jpeg')}}" class="d-block w-100" alt="...">
        <div class="carousel-caption d-none d-md-block">
          <h5 style="color: black">ARDUINO MKR WIFI 1010</h5>

        </div>
      </div>
      <div class="carousel-item" style= "width: 100%;margin-top: 100px;">
        <img src="{{asset('img/estructuraServidorWeb.png')}}" class="d-block w-100" alt="...">
        <div class="carousel-caption d-none d-md-block"  style= "height: 100%; margin-top: 200px;">
          <h5 style="color: black">NETWORK - APPLICATION</h5>

        </div>
      </div>
    </div>
    <a class="carousel-control-prev" href="#carouselExampleCaptions" role="button" data-slide="prev">
      <span class="carousel-control-prev-icon" aria-hidden="true"></span>
      <span class="sr-only">Previous</span>
    </a>
    <a class="carousel-control-next" href="#carouselExampleCaptions" role="button" data-slide="next">
      <span class="carousel-control-next-icon" aria-hidden="true"></span>
      <span class="sr-only">Next</span>
    </a>
  </div>
</div>







    <!-- *************************************************NOTE: Acaba el contenido************************************************* -->
    <!-- Optional JavaScript -->
    <!-- jQuery first, then Popper.js, then Bootstrap JS -->
    <script src="https://code.jquery.com/jquery-3.3.1.slim.min.js" integrity="sha384-q8i/X+965DzO0rT7abK41JStQIAqVgRVzpbzo5smXKp4YfRvH+8abtTE1Pi6jizo" crossorigin="anonymous"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.7/umd/popper.min.js" integrity="sha384-UO2eT0CpHqdSJQ6hJty5KVphtPhzWj9WO1clHTMGa3JDZwrnQq4sF86dIHNDz0W1" crossorigin="anonymous"></script>
    <script src="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/js/bootstrap.min.js" integrity="sha384-JjSmVgyd0p3pXB1rRibZUAYoIIy6OrQ6VrjIEaFf/nJGzIxFDsf4x0xIM+B07jRM" crossorigin="anonymous"></script>
  </body>
</html>
