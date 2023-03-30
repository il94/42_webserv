// SELECTEURS
// document.querySelector('h4').style.background = "yellow";

// Click event
const questionContainer = document.querySelector(".click-event");
const btn1 = document.querySelector("#btn-1");
const btn2 = document.getElementById("btn-2");
const reponse = document.querySelector("p");

questionContainer.addEventListener('click', () =>{
    questionContainer.classList.add('question-clicked');
});

btn1.addEventListener('click', () =>{
    reponse.classList.add('show-response')
    reponse.style.background = "green";
});

btn2.addEventListener('click', () => {
    reponse.classList.add('show-response')
    reponse.style.background = "red";

});
//------------------------------------------------------
// Mouse Events
const mouse = document.querySelector('.mousemove');

window.addEventListener('mousemove', (e) => {
    mouse.style.left = e.pageX + "px";
    mouse.style.top = e.pageY + "px"; 
});

window.addEventListener("mousedown", () => {
    console.log("salut");
});

questionContainer.addEventListener('mousenter',() => {
    questionContainer.style.background = "rgba(0,0,0,0.6)";
})

questionContainer.addEventListener('mouseout',() => {
    questionContainer.style.background = "rgb(0,0,0)";
})

reponse.addEventListener("mouseover", ()=> {
    reponse.style.transform = "rotate(2deg)"
})
//--------------------------------------------------------
//Keypress event

const keypress = document.querySelector(".keypress");
const key = document.getElementById("key");

document.addEventListener('keypress', (e) => {
    key.textContent = e.key;
    keypress.style.background ="blue";
})
//--------------------------------------------------------
// scroll event

const nav = document.querySelector("nav");

window.addEventListener("scroll", () =>{
    if(window.scrollY > 120){
        nav.style.top = 0;
    }
    else{
        nav.style.top = "-50px"
    }
})
//--------------------------------------------------------

const inputName = document.querySelector('input[type="text"]');
const select = document.querySelector('select');
const form = document.querySelector("form");
let pseudo = "";
let language = "";

inputName.addEventListener('input', (e) =>{
    pseudo = e.target.value;
    console.log(pseudo)
});

select.addEventListener('input', (e)=> {
    language = e.target.value;
});

form.addEventListener('submit', (e) =>{
    e.preventDefault();

    if (cgv.checked){
        document.querySelector('form > div').innerHTML = `
        <h3>Pseudo : ${pseudo} </h3>
        <h4> Langage préféré : ${language} </h4>
        `;
    }else {
        alert('CVG...!')
    }
});
// --------------------------------------------------------------
// Load event 
window.addEventListener("load", () =>{
    console.log('Document chargé');
});
// --------------------------------------------------
// foreach

const boxes = document.querySelectorAll(".box");

boxes.forEach((box) =>{
    box.addEventListener("click", (e) => {
        e.target.style.transform = "scale(0.7)";
    });
    box.addEventListener("mouseout", (e) => {
        e.target.style.transform = "scale(1)";
    });

});

//--------------------------------------------------

//SetProperty
window;addEventListener('mousemove', (e) => {
    nav.style.setProperty('--x', e.layerX + "px")
    nav.style.setProperty('--y', e.layerY + "px")
})