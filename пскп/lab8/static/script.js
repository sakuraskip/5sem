document.getElementById('loadJson').addEventListener('click', () => {
    fetch('/data.json')
        .then(resp => resp.json())
        .then(data => {
            document.getElementById('json').textContent = JSON.stringify(data, null, 2);
        })
});
document.getElementById('loadXml').addEventListener('click', () => {
    fetch('/data.xml')
        .then(resp => resp.text())
        .then(str => {
            document.getElementById('xml').textContent = str;
        })
    });