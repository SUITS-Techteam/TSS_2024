function loadUIA(){

    $.getJSON("json_data/UIA.json", function( data ){
        if (data.uia.eva1_power == true){
            document.getElementById("eva1PowerSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("uia_eva1_power_switch").checked = true;
        }
        else {
            document.getElementById("eva1PowerSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("uia_eva1_power_switch").checked = false;
        }
        if (data.uia.eva2_power == true){
            document.getElementById("eva2PowerSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("uia_eva2_power_switch").checked = true;
        }
        else {
            document.getElementById("eva2PowerSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("uia_eva2_power_switch").checked = false;
        }

        if (data.uia.eva1_water_supply == true){
            document.getElementById("eva1WaterSupplySensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("uia_eva1_water_supply_switch").checked = true;
        }
        else {
            document.getElementById("eva1WaterSupplySensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("uia_eva1_water_supply_switch").checked = false;
        }
        if (data.uia.eva2_water_supply == true){
            document.getElementById("eva2WaterSupplySensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("uia_eva2_water_supply_switch").checked = true;
        }
        else {
            document.getElementById("eva2WaterSupplySensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("uia_eva2_water_supply_switch").checked = false;
        }

        if (data.uia.eva1_water_waste == true){
            document.getElementById("eva1WaterWasteSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("uia_eva1_water_waste_switch").checked = true;
        }
        else {
            document.getElementById("eva1WaterWasteSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("uia_eva1_water_waste_switch").checked = false;
        }
        if (data.uia.eva2_water_waste == true){
            document.getElementById("eva2WaterWasteSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("uia_eva2_water_waste_switch").checked = true;
        }
        else {
            document.getElementById("eva2WaterWasteSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("uia_eva2_water_waste_switch").checked = false;
        }

        if (data.uia.eva1_oxy == true){
            document.getElementById("eva1OxygenSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("uia_eva1_oxy_switch").checked = true;
        }
        else {
            document.getElementById("eva1OxygenSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("uia_eva1_oxy_switch").checked = false;
        }
        if (data.uia.eva2_oxy == true){
            document.getElementById("eva2OxygenSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("uia_eva2_oxy_switch").checked = true;
        }
        else {
            document.getElementById("eva2OxygenSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("uia_eva2_oxy_switch").checked = false;
        }

        if (data.uia.oxy_vent == true){
            document.getElementById("oxygenVentSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("uia_oxy_vent_switch").checked = true;
        }
        else {
            document.getElementById("oxygenVentSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("uia_oxy_vent_switch").checked = false;
        }
        if (data.uia.depress == true){
            document.getElementById("depressPumpSensor").style.backgroundColor = 'rgba(0, 240, 10, 1)';
            document.getElementById("uia_depress_switch").checked = true;
        }
        else {
            document.getElementById("depressPumpSensor").style.backgroundColor = 'rgba(100, 100, 100, 1)';
            document.getElementById("uia_depress_switch").checked = false;
        }
    });
}

//Runs on load of the page
function onload() {

    // immediately load
    loadUIA();

    // Continuously refreshes values from the UIA
	setInterval(function() {
        
        loadUIA();
        
    }, 1000);
}