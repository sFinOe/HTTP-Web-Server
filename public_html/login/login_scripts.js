const signUpButton = document.getElementById('signUp');
const signInButton = document.getElementById('signIn');
const container = document.getElementById('container');
const login = document.getElementById('email_login');
const password = document.getElementById('password_login');
const r_name = document.getElementById('name_register');
const r_email = document.getElementById('email_register');
const r_password = document.getElementById('password_register');
const sign_in = document.getElementById('sign_in_btn');
const sign_up = document.getElementById('sign_up_btn');



function ValidateEmail(mail) 
{
	console.log("anything hererere");
 if (/^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/.test(login.value)){
    return (true)
  }
    alert("You have entered an invalid email address!")
    return (false)
}

function Validate_r_Email(mail) 
{
	console.log("anything hererere");
 if (/^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/.test(r_email.value)){
    return (true)
  }
    alert("You have entered an invalid email address!")
    return (false)
}

signUpButton.addEventListener('click', () => {
	container.classList.add("right-panel-active");
});

signInButton.addEventListener('click', () => {
	container.classList.remove("right-panel-active");
});


 // sign in button

sign_in.addEventListener('click', () => {
	if(ValidateEmail(login)){
		var pathArray = window.location.pathname.split('/');
		var newPathname = "";
		for (i = 0; i < pathArray.length; i++) {
		  newPathname += "/";
		  newPathname += pathArray[i];
		}
	//   var newURL = window.location.protocol + "/" + window.location.host + "/" + newPathname;
		const url = `${window.location.origin}/cgi-bin/valid_login.js`;
		let formData = new FormData();
		formData.append('login', login.value + " " + password.value);
		fetch(url, {
			method: 'POST',
			body: formData
		})
		.then(Response => {
			if(Response.status == 200){
				window.location.href = `${window.location.origin}/`;
			}
		  })
	}
});

// sign up button

sign_up.addEventListener('click', () => {
	
	if (Validate_r_Email(r_email)){
		var pathArray = window.location.pathname.split('/');
		var newPathname = "";
		for (i = 0; i < pathArray.length; i++) {
		  newPathname += "/";
		  newPathname += pathArray[i];
		}
		const url = `${window.location.origin}/cgi-bin/valid_login.js`;
		let formData = new FormData();
		formData.append('signup', r_name.value + " " + r_email.value + " " + r_password.value);
		fetch(url, {
			method: 'POST',
			body: formData
		})
		.then(res => {
			// if (res.status === 200){
			// 	window.location = res.redirect;
			//   }
			// else{
			// 	alert(res.message);
			// }
		})
	}
});



