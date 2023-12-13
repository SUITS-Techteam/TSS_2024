function loadCOMM(){

    $.getJSON("json_data/COMM.json", function( data ){

        // console.log(data);
        if (data.comm.comm_tower == true){
            document.getElementById("commSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("comm_switch").checked = true;
        }
        else {
            document.getElementById("commSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("comm_switch").checked = false;
        }
    });
}

//Runs on load of the page
function onload() {

    // Load immediately
    loadCOMM();

    // Continuously refreshes values from the COMM Tower
	setInterval(function() {

        loadCOMM();

    }, 1000);
}