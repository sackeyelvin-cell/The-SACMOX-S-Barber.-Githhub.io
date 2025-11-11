Sacmoxs Barber - Website Starter Package
=======================================

Contents:
- package.json (example)
- src/App.jsx (main React component)
- README.md (this file)

What I built:
- Responsive React + Tailwind single-file app (App.jsx) that includes:
  * Booking form with location zones and service selection
  * Invoice preview + PDF download
  * WhatsApp prefilled booking link using your number (+233500283177)
  * Admin dashboard scaffold using Firebase Firestore (optional)
  * Gallery and Reviews placeholders
  * Service areas iframe

Quick start (no Firebase, fastest launch):
1. Create a Vite React app: npx create-vite@latest sacmoxs --template react
2. Copy src/App.jsx from this package into your project's src/App.jsx (replace existing)
3. Install dependencies: npm install jspdf
4. Add Tailwind or use default CSS for now. For fastest test, remove Tailwind classes or use CDN.
5. Run: npm run dev
6. Deploy to Netlify/Vercel by connecting your repo.

Recommended (small cost, better experience):
- Add Tailwind CSS for the design: https://tailwindcss.com/docs/guides/vite
- Create a free Firebase project and paste your firebaseConfig into src/App.jsx to enable live bookings.
- Use Netlify/Vercel for free hosting.

Files in this zip:
- src/App.jsx (main app)
- README.md

Notes:
- The app uses your WhatsApp number: +233500283177. If incorrect, edit BUSINESS_PHONE in src/App.jsx.
- Firebase is optional. If you don't want a database, the app will still function as a local booking + WhatsApp link generator.
- I recommend launching without Firebase first (zero cost), then add Firebase when you can create a free account.

