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
        // document.getElementById("eva2_rock_name").innerText = data.spec.eva2.name;
        document.getElementById("eva2_sio2").innerText      = data.spec.eva2.data.SiO2;
        document.getElementById("eva2_tio2").innerText      = data.spec.eva2.data.TiO2;
        document.getElementById("eva2_al2o3").innerText     = data.spec.eva2.data.Al2O3;
        document.getElementById("eva2_feo").innerText       = data.spec.eva2.data.FeO;
        document.getElementById("eva2_mno").innerText       = data.spec.eva2.data.MnO;
        document.getElementById("eva2_mgo").innerText       = data.spec.eva2.data.MgO;
        document.getElementById("eva2_cao").innerText       = data.spec.eva2.data.CaO;
        document.getElementById("eva2_k2o").innerText       = data.spec.eva2.data.K2O;
        document.getElementById("eva2_p2o3").innerText      = data.spec.eva2.data.P2O3;
        // document.getElementById("dropdownEV2").value        = data.spec.eva2.id;
    });
}

function onload() {

    // Load immediately
    loadSPEC();

    // Continuously refreshes values from the EVA 2 SPEC
	setInterval(function() {

        loadSPEC();

    }, 1000);
}