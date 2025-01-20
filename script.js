// Array to store guest names
let guestList = [];

// Get the form and guest list elements
const guestForm = document.getElementById("guestForm");
const guestNameInput = document.getElementById("guestName");
const guestListContainer = document.getElementById("guestList");

// Function to update the guest list on the page
function updateGuestList() {
    guestListContainer.innerHTML = '';  // Clear the list before updating
    guestList.forEach((guest, index) => {
        const listItem = document.createElement("li");
        listItem.textContent = guest;
        guestListContainer.appendChild(listItem);
    });
}

// Event listener for form submission
guestForm.addEventListener("submit", function(event) {
    event.preventDefault();
    const guestName = guestNameInput.value.trim();

    if (guestName) {
        guestList.push(guestName);  // Add the name to the list
        guestNameInput.value = '';  // Clear the input field
        updateGuestList();  // Update the guest list display
    }
});
