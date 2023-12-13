// global variable for UI elements
let video = document.getElementById('videoFeed');
let map = document.getElementById('rockYardMap');

// Loads the rover video feed
function loadVideoFeed() {
    video.src = "http://192.168.51.194:8080/stream?topic=/camera/image_raw&type=ros_compressed";
}

// Loads the rock yard maps
function loadMap() {
    map.src = "./images/rockYardMap-min.png";
}

//Runs on load of the page
function onload() {

    // Init all the variables
    video = document.getElementById('videoFeed');
    map = document.getElementById('rockYardMap');

    loadMap();

    // Continuously refreshes for the video feed
    setInterval(function() {
        loadVideoFeed();
    }, 1000);
}