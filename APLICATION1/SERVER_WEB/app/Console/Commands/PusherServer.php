<?php


namespace App\Console\Commands;

use Illuminate\Console\Command;

use Ratchet\Server\IoServer;
use Ratchet\Http\HttpServer;
use Ratchet\WebSocket\WsServer;


use App\Socket\Pusher;
use React\EventLoop\Factory as ReactLoop;
use React\ZMQ\Context as ReactContext;
use React\Socket\Server as ReactServer;
use Ratchet\Wamp\WampServer;

class PusherServer extends Command
{
    /**
     * The name and signature of the console command.
     *
     * @var string
     */
    protected $signature = 'socketpush:serve';

    /**
     * The console command description.
     *
     * @var string
     */
    protected $description = 'Command description';

    /**
     * Create a new command instance.
     *
     * @return void
     */
    public function __construct()
    {
        parent::__construct();
    }

    /**
     * Execute the console command.
     *
     * @return mixed
     */
    public function handle()
    {
      $loop   = ReactLoop::create();
      $pusher = new Pusher;

      // Listen for the web server to make a ZeroMQ push after an ajax request
      $context = new ReactContext($loop);
      $pull = $context->getSocket(\ZMQ::SOCKET_PULL);
      $pull->bind('tcp://192.168.0.159:5555'); // Binding to 127.0.0.1 means the only client that can connect is itself
      $pull->on('message', array($pusher, 'broadcast'));

      // Set up our WebSocket server for clients wanting real-time updates
      $webSock = new ReactServer('0.0.0.0:8080', $loop); // Binding to 0.0.0.0 means remotes can connect
      $webServer = new IoServer(
        new HttpServer(
            new WsServer(
                new WampServer(
                    $pusher
                )
            )
        ),
        $webSock
      );
      $this->info("Run handle");

      $loop->run();
    }
}
