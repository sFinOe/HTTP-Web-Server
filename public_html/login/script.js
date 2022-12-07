const switchers = [...document.querySelectorAll('.switcher')]
const login_email = document.querySelector('#login-email')
const login_password = document.querySelector('#login-password')
const login_submit = document.querySelector('#login-submit')
const signup_email = document.querySelector('#signup-email')
const signup_password = document.querySelector('#signup-password')
const signup_submit = document.querySelector('#signup-submit')

switchers.forEach(item => {
	item.addEventListener('click', function() {
		switchers.forEach(item => item.parentElement.classList.remove('is-active'))
		this.parentElement.classList.add('is-active')
	})
})

login_submit.addEventListener('click', function(e) {
	e.preventDefault()
	if (login_email.value.length > 0 && login_password.value.length > 0) {
		let formData = new FormData()
		console.log(login_email.value)
		formData.append('login', login_email.value + " " + login_password.value)
		console.log(login_password.value);
		const url = `${window.location.origin}/cgi-bin/login_checker.py`;
		fetch(url, {
			method: 'post',
			body: formData
		})
		.then(res => {
			if (res.status === 200){
				window.location.reload();
		}})

	} else {
		alert('Please fill in all fields')
	}
})

signup_submit.addEventListener('click', function(e) {
	e.preventDefault()
	if (signup_email.value.length > 0 && signup_password.value.length > 0) {
		let formData = new FormData()
		formData.append('signup', signup_email.value + " " + signup_password.value)
		const url = `${window.location.origin}/login/`;
		fetch(url, {
			method: 'POST',
			body: formData
		})
		.then(res => {
			if (res.status === 200){
				window.location.reload();
		}})

	} else {
		alert('Please fill in all fields')
	}
})