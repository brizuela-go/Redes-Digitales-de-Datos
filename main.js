const loggedOutLinks = document.querySelectorAll(".logged-out");
const loggedInLinks = document.querySelectorAll(".logged-in");
const displayUserInfo = document.getElementById("displayUserInfo");
const displaySensoresInfo = document.getElementById("displaySensoresInfo");
const loginRegister = document.getElementById("login-register");
const mySignInAlert = document.getElementById("mySignInAlert");
const mySignUpAlert = document.getElementById("mySignUpAlert");

const loginCheck = (user) => {
  if (user) {
    loggedInLinks.forEach((link) => (link.style.display = "block"));
    loggedOutLinks.forEach((link) => (link.style.display = "none"));
    loginRegister.style.display = "none";
  } else {
    loggedInLinks.forEach((link) => (link.style.display = "none"));
    loggedOutLinks.forEach((link) => (link.style.display = "block"));
    loginRegister.style.display = "block";
  }
};

// SignUp
const signUpForm = document.getElementById("signup-form");
signUpForm.addEventListener("submit", (e) => {
  e.preventDefault();
  const email = signUpForm["registerEmail"].value;
  const password = signUpForm["registerPassword"].value;
  const repeatedPassword = signUpForm["registerRepeatPassword"].value;

  if (password == repeatedPassword) {
    // Authenticate the User
    auth
      .createUserWithEmailAndPassword(email, password)
      .then((userCredential) => {
        mySignUpAlert.style.display = "none";
      })
      .catch((error) => {
        const errorMessage = error.message;
        mySignUpAlert.style.display = "block";
        mySignUpAlert.setAttribute("class", "alert alert-danger my-3");
        mySignUpAlert.setAttribute("role", "alert");
        mySignUpAlert.innerHTML = `<div>
        <i class="fas fa-times-circle me-3"></i>${errorMessage}
      </div>`;
      });
  } else {
    alert("Las contraseñas, no coinciden...");
  }
});

// Logout
const logout = document.querySelector("#logout");

logout.addEventListener("click", (e) => {
  e.preventDefault();
  auth.signOut().then(() => {
    console.log("signup out");
  });
});

// SingIn
const signInForm = document.querySelector("#login-form");

signInForm.addEventListener("submit", (e) => {
  e.preventDefault();
  const email = signInForm["loginName"].value;
  const password = signInForm["loginPassword"].value;

  // Authenticate the User
  auth
    .signInWithEmailAndPassword(email, password)
    .then((userCredential) => {
      mySignInAlert.style.display = "none";
    })
    .catch((error) => {
      const errorMessage = error.message;
      mySignInAlert.style.display = "block";
      mySignInAlert.setAttribute("class", "alert alert-danger my-3");
      mySignInAlert.setAttribute("role", "alert");
      mySignInAlert.innerHTML = `<div>
    <i class="fas fa-times-circle me-3"></i>${errorMessage}
  </div>`;
    });
});

// events
// list for auth state changes
auth.onAuthStateChanged((user) => {
  if (user) {
    console.log("signin");
    user.displayName == null
      ? (displayUserInfo.innerHTML = ` <img class="rounded-circle" src="https://media.istockphoto.com/vectors/user-icon-flat-isolated-on-white-background-user-symbol-vector-vector-id1300845620?k=20&m=1300845620&s=612x612&w=0&h=f4XTZDAv7NPuZbG0habSpU0sNgECM0X7nbKzTUta3n8=" alt="avatar" style="width: 10rem;"/><h1 class="my-4">¡Bienvenido, ${user.email}!</h1>`)
      : (displayUserInfo.innerHTML = `<img class="rounded-circle mt-2" src="${user.photoURL}" alt="avatar"/> <h1 class="my-5">¡Bienvenido, ${user.displayName}!</h1>`);

    loginCheck(user);
    displaySensoresInfo.style.display = "flex";
  } else {
    displayUserInfo.innerHTML = "";
    displaySensoresInfo.style.display = "none";
    console.log("signout");
    loginCheck(user);
  }
});

// Login with Google
const googleButton = document.querySelector("#googleLogin");
const googleButtonRegister = document.querySelector("#googleRegister");

googleButton.addEventListener("click", (e) => {
  e.preventDefault();

  const provider = new firebase.auth.GoogleAuthProvider();
  auth
    .signInWithPopup(provider)
    .then((result) => {
      console.log("google sign in");
      mySignInAlert.style.display = "none";
      mySignUpAlert.style.display = "none";
    })
    .catch((err) => {
      console.log(err);
    });
});

googleButtonRegister.addEventListener("click", (e) => {
  e.preventDefault();

  const provider = new firebase.auth.GoogleAuthProvider();
  auth
    .signInWithPopup(provider)
    .then((result) => {
      mySignInAlert.style.display = "none";
      mySignUpAlert.style.display = "none";
      console.log("google sign in");
    })
    .catch((err) => {
      console.log(err);
    });
});

// Acelerómetro
const Acelerometro = db.ref("acelerometro");
const prueba = document.getElementById("prueba");

Acelerometro.on("value", (snapshot) => {
  prueba.innerHTML = "";

  snapshot.forEach((element) => {
    prueba.innerText += element.val();
  });
});

// Keypad
const Keypad = db.ref("keypad");
const keypad = document.getElementById("keypad");

Keypad.on("value", (snapshot) => {
  keypad.innerHTML = "";

  snapshot.forEach((element) => {
    keypad.innerText += `Tecla:  ${element.val()}`;
  });
});

// Potenciómetro
const Potenciometro = db.ref("potenciometro");
const potenciometro = document.getElementById("potenciometro");

Potenciometro.on("value", (snapshot) => {
  potenciometro.innerHTML = "";

  snapshot.forEach((element) => {
    potenciometro.innerText += `${element.val()}`;
  });
});

// Sonido
const detectorDeSonido = db.ref("sonido");
const detector = document.getElementById("detector-de-sonido");
const speaker = document.getElementById("speaker");

detectorDeSonido.on("value", (snapshot) => {
  detector.innerHTML = "";

  snapshot.forEach((element) => {
    detector.innerText += `${element.val()}`;
    speaker.innerHTML = `<i class="fas fa-volume-off"></i>`;
    element.val() == "Silencio"
      ? (speaker.innerHTML = `<i class="fas fa-volume-off"></i>`)
      : (speaker.innerHTML = `<i class="fas fa-volume-up text-success animate__animated animate__pulse animate__infinite	infinite"></i>`);
  });
});

// CO2
const co2 = db.ref("aire");
const detectorCo2 = document.getElementById("co2");
const wind = document.getElementById("wind");

co2.on("value", (snapshot) => {
  detectorCo2.innerHTML = "";

  detectorCo2.innerText += `${snapshot.val()} ppm`;
  wind.innerHTML = `<i class="fas fa-wind"></i>`;
});

//Luz
const luz = db.ref("luz");
const detectorLuz = document.getElementById("luz");
const foco = document.getElementById("foco");

luz.on("value", (snapshot) => {
  detectorLuz.innerHTML = "";

  detectorLuz.innerText += `${snapshot.val().toFixed(4)} lúmenes`;
  foco.innerHTML = `<i class="far fa-lightbulb"></i>`;
});
