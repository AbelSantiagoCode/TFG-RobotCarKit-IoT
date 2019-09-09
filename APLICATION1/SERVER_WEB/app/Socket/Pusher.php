<?php
namespace App\Socket;

use Ratchet\ConnectionInterface;
use Ratchet\Wamp\WampServerInterface;


//use App\Socket\Base\BasePusher as Base;
use ZMQContext;


/**
 * PUSHER
 */
class Pusher implements WampServerInterface
{

    /**
   * A lookup of all the topics clients have subscribed to
   */
  protected $subscribedTopics = array();

  public function getSubscribedTopics()
  {
    return $this->subscribedTopics;
  }

  public function addSubscribedTopic($topic)
  {
    $this->subscribedTopics[$topic->getId()] = $topic;
  }

  public function onSubscribe(ConnectionInterface $conn, $topic) {
      $this->subscribedTopics[$topic->getId()] = $topic;
  }
  public function onUnSubscribe(ConnectionInterface $conn, $topic) {
  }
  public function onOpen(ConnectionInterface $conn) {
    echo "New connection! ({$conn->resourceId})\n";
  }
  public function onClose(ConnectionInterface $conn) {
    echo "Connection {$conn->resourceId} has disconnected\n";
  }
  public function onCall(ConnectionInterface $conn, $id, $topic, array $params) {
      // In this application if clients send data it's because the user hacked around in console
      $conn->callError($id, $topic, 'You are not allowed to make calls')->close();
  }
  public function onPublish(ConnectionInterface $conn, $topic, $event, array $exclude, array $eligible) {
      // In this application if clients send data it's because the user hacked around in console
      $conn->close();
  }
  public function onError(ConnectionInterface $conn, \Exception $e) {
  }

  /**
   * @param string JSON'ified string we'll receive from ZeroMQ
   */
  public function onBlogEntry($entry) {
      $entryData = json_decode($entry, true);

      // If the lookup topic object isn't set there is no one to publish to
      if (!array_key_exists($entryData['category'], $this->subscribedTopics)) {
          return;
      }

      $topic = $this->subscribedTopics[$entryData['category']];

      // re-send the data to all the clients subscribed to that category
      $topic->broadcast($entryData);
  }

  /*
  * Peer to comunicate between Controller and PusherServer
  */
  static function sentDataToServer(array $data)
  {
    // This is our new stuff
    /*
    protected function alertMessage()
    {
      $data = [
        'topic_id' => 'onAlertMessage',
        'data' => 'someData'
      ];
      \App\Socket\Pusher::sentDataToServer($data);

    }
    */
    $context = new ZMQContext();
    $socket = $context->getSocket(\ZMQ::SOCKET_PUSH, 'my pusher');
    $socket->connect("tcp://192.168.0.159:5555"); // maybe another port.

    $socket->send(json_encode($data));
  }

  public function broadcast($jsonDataToSend)
  {
    $aDataToSend = json_decode($jsonDataToSend,true);
    $subscribedTopics = $this->getSubscribedTopics();
    if (isset($subscribedTopics[$aDataToSend['topic_id']])) {
      $topic = $subscribedTopics[$aDataToSend['topic_id']];
      $topic->broadcast($aDataToSend);
    }
  }
}
