/* When the user clicks on the button,
toggle between hiding and showing the dropdown content */
function dropdownEVA1() {
    document.getElementById("dropdownEV1").classList.toggle("show");
}

function dropdownEVA2() {
    document.getElementById("dropdownEV2").classList.toggle("show");
}
  
// Close the dropdown menu if the user clicks outside of it
// window.onclick = function(event) {
//     if (!event.target.matches('.dropbtn')) {
//         var dropdowns = document.getElementsByClassName("dropdown-content");
//         var i;
//         for (i = input; i < dropdowns.length; i++) {
//             var openDropdown = dropdowns[i];
//             if (openDropdown.classList.contains('show')) {
//                 openDropdown.classList.remove('show');
//             }
//         }
//     }
// }

function loadSPEC() {
    $.getJSON("json_data/SPEC.json", function( data ){
        // console.log(data);
        document.getElementById("eva1_rock_name").innerText = data.spec.eva1.name;
        document.getElementById("eva1_sio2").innerText      = data.spec.eva1.data.SiO2;
        document.getElementById("eva1_tio2").innerText      = data.spec.eva1.data.TiO2;
        document.getElementById("eva1_al2o3").innerText     = data.spec.eva1.data.Al2O3;
        document.getElementById("eva1_feo").innerText       = data.spec.eva1.data.FeO;
        document.getElementById("eva1_mno").innerText       = data.spec.eva1.data.MnO;
        document.getElementById("eva1_mgo").innerText       = data.spec.eva1.data.MgO;
        document.getElementById("eva1_cao").innerText       = data.spec.eva1.data.CaO;
        document.getElementById("eva1_k2o").innerText       = data.spec.eva1.data.K2O;
        document.getElementById("eva1_p2o3").innerText      = data.spec.eva1.data.P2O3;
        document.getElementById("eva1_other").innerText     = data.spec.eva1.data.other;
        document.getElementById("dropdownEV1").value        = data.spec.eva1.id;

        document.getElementById("eva2_rock_name").innerText = data.spec.eva2.name;
        document.getElementById("eva2_sio2").innerText      = data.spec.eva2.data.SiO2;
        document.getElementById("eva2_tio2").innerText      = data.spec.eva2.data.TiO2;
        document.getElementById("eva2_al2o3").innerText     = data.spec.eva2.data.Al2O3;
        document.getElementById("eva2_feo").innerText       = data.spec.eva2.data.FeO;
        document.getElementById("eva2_mno").innerText       = data.spec.eva2.data.MnO;
        document.getElementById("eva2_mgo").innerText       = data.spec.eva2.data.MgO;
        document.getElementById("eva2_cao").innerText       = data.spec.eva2.data.CaO;
        document.getElementById("eva2_k2o").innerText       = data.spec.eva2.data.K2O;
        document.getElementById("eva2_p2o3").innerText      = data.spec.eva2.data.P2O3;
        document.getElementById("eva2_other").innerText     = data.spec.eva2.data.other;
        document.getElementById("dropdownEV2").value        = data.spec.eva2.id;
    });
}

function onload() {

    // Load immediately
    loadSPEC();

    // Continuously refreshes values from both EVA 1 and EVA 2 SPECs
	setInterval(function() {

        loadSPEC();

    }, 1000);
}