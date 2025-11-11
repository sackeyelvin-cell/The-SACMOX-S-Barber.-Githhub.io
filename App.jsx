// Sacmoxs Barber — Full Website (React + Tailwind + Firebase)
// Single-file starter with integrated Phase 1 + Phase 2 features.
// -------------------------------
// HOW TO USE
// 1) Create a React app (Vite recommended) and install Tailwind CSS.
// 2) Install required packages:
//    npm install firebase react-router-dom jsPDF
// 3) Replace the Firebase config placeholders with your Firebase project's config.
// 4) Update BUSINESS_PHONE in this file to your WhatsApp number in international format.
// 5) Run: npm run dev and deploy to Netlify/Vercel when ready.
// -------------------------------

import React, { useEffect, useState } from 'react';
import { initializeApp } from 'firebase/app';
import {
  getFirestore,
  collection,
  addDoc,
  query,
  orderBy,
  onSnapshot,
  serverTimestamp,
  updateDoc,
  doc,
} from 'firebase/firestore';
import jsPDF from 'jspdf';

// ================== CONFIG — EDIT THESE ==================
const BUSINESS_PHONE = '+233500283177'; // sacmoxs WhatsApp (user-provided 0500283177 -> +233500283177)
const SERVICE_CITY = 'Accra and environs (will travel further for extra cost)';

// Firebase config — you MUST replace with your project's config after creating a Firebase project.
const firebaseConfig = {
  apiKey: 'REPLACE_WITH_YOUR_API_KEY',
  authDomain: 'REPLACE_WITH_YOUR_AUTH_DOMAIN',
  projectId: 'REPLACE_WITH_YOUR_PROJECT_ID',
  storageBucket: 'REPLACE_WITH_YOUR_STORAGE_BUCKET',
  messagingSenderId: 'REPLACE_WITH_YOUR_MESSAGING_SENDER_ID',
  appId: 'REPLACE_WITH_YOUR_APP_ID',
};

// =========================================================

// Initialize Firebase app and Firestore (safe to call even if placeholders exist but will fail until replaced)
let db = null;
try {
  const app = initializeApp(firebaseConfig);
  db = getFirestore(app);
} catch (e) {
  console.warn('Firebase not initialized — replace firebaseConfig with your credentials to enable DB.');
}

// ---------------- Pricing & Zones ----------------
const ZONES = {
  within3: { label: 'Within 3 km (same area)', fee: 10 },
  nearby: { label: 'Nearby zones (5-10 km)', fee: 15 },
  farther: { label: 'Farther zones (10+ km)', fee: 20 },
};

const SERVICE_PRICES = {
  haircut: 50,
  beard: 20,
  fatherAndSon: 100,
  treatment: 30,
  student: 45,
};

// ---------------- Utilities ----------------
function ghs(n) {
  return `GHS ${n.toFixed(2)}`;
}

function whatsappLinkFromForm(f) {
  const services = [];
  if (f.service.fatherAndSon) services.push('Father & Son');
  else {
    if (f.service.haircut) services.push('Haircut');
    if (f.service.beard) services.push('Beard');
    if (f.service.treatment) services.push('Treatment');
    if (f.service.student) services.push('Student Discount');
  }
  const zone = ZONES[f.zone].label;
  const msg = `Hi Sacmoxs Barber!%0A%0AName:%20${encodeURIComponent(f.name)}%0APhone:%20${encodeURIComponent(f.phone)}%0AService:%20${encodeURIComponent(services.join(', '))}%0ALocation:%20${encodeURIComponent(zone)}%0ADate:%20${encodeURIComponent(f.date)}%0ATime:%20${encodeURIComponent(f.time)}%0ANotes:%20${encodeURIComponent(f.notes)}%0A%0AThank%20you!`;
  // WhatsApp link with business phone
  const cleanPhone = BUSINESS_PHONE.replace(/[^0-9+]/g, '');
  const phoneForLink = cleanPhone.startsWith('+') ? cleanPhone.replace('+', '') : cleanPhone;
  return `https://wa.me/${phoneForLink}?text=${msg}`;
}

function calcInvoice(form) {
  const items = [];
  if (form.service.fatherAndSon) {
    items.push({ desc: 'Father & Son Package', qty: 1, unit: SERVICE_PRICES.fatherAndSon, total: SERVICE_PRICES.fatherAndSon });
  } else {
    if (form.service.haircut) {
      const price = form.service.student ? SERVICE_PRICES.student : SERVICE_PRICES.haircut;
      items.push({ desc: form.service.student ? 'Student Haircut' : 'Haircut', qty: 1, unit: price, total: price });
    }
    if (form.service.beard) {
      items.push({ desc: 'Beard Grooming', qty: 1, unit: SERVICE_PRICES.beard, total: SERVICE_PRICES.beard });
    }
    if (form.service.treatment) {
      items.push({ desc: 'Add-on: Treatment', qty: 1, unit: SERVICE_PRICES.treatment, total: SERVICE_PRICES.treatment });
    }
  }
  const distance = ZONES[form.zone] ? ZONES[form.zone].fee : 10;
  if (items.length === 0) items.push({ desc: 'Haircut (default)', qty: 1, unit: SERVICE_PRICES.haircut, total: SERVICE_PRICES.haircut });
  items.push({ desc: `Distance Fee (${ZONES[form.zone].label})`, qty: 1, unit: distance, total: distance });
  const subtotal = items.reduce((s, it) => s + it.total, 0);
  const discount = 0; // reserved for loyalty promo
  const total = subtotal - discount;
  return { items, subtotal, discount, total };
}

// ---------------- React App ----------------
export default function SacmoxsFullApp() {
  const [form, setForm] = useState({
    name: '',
    phone: '',
    date: '',
    time: '',
    zone: 'within3',
    service: { haircut: true, beard: false, fatherAndSon: false, treatment: false, student: false },
    notes: '',
  });

  const [invoice, setInvoice] = useState(null);
  const [bookings, setBookings] = useState([]); // admin bookings list
  const [adminMode, setAdminMode] = useState(false); // toggle admin dashboard
  const [message, setMessage] = useState('');

  useEffect(() => {
    if (!db) return;
    // Subscribe to bookings collection for admin dashboard
    const q = query(collection(db, 'bookings'), orderBy('createdAt', 'desc'));
    const unsub = onSnapshot(q, (snap) => {
      const arr = [];
      snap.forEach((d) => arr.push({ id: d.id, ...d.data() }));
      setBookings(arr);
    });
    return () => unsub();
  }, []);

  function updateForm(part) {
    setForm((f) => ({ ...f, ...part }));
  }

  function toggleService(key) {
    setForm((f) => ({ ...f, service: { ...f.service, [key]: !f.service[key] } }));
  }

  async function submitBooking(e) {
    e && e.preventDefault();
    const inv = calcInvoice(form);
    setInvoice(inv);
    // Save to Firebase if configured
    if (db) {
      try {
        await addDoc(collection(db, 'bookings'), {
          name: form.name,
          phone: form.phone,
          date: form.date,
          time: form.time,
          zone: form.zone,
          service: form.service,
          notes: form.notes,
          invoice: inv,
          status: 'pending',
          createdAt: serverTimestamp(),
        });
        setMessage('Booking saved. Confirm via WhatsApp to finalize.');
      } catch (err) {
        console.error(err);
        setMessage('Failed to save booking to database — check firebase config.');
      }
    } else {
      setMessage('Preview ready. Firebase not configured — this is a local preview.');
    }
  }

  async function confirmAndSend() {
    // Create DB entry if not created
    if (db) {
      try {
        await addDoc(collection(db, 'confirmedBookings'), {
          name: form.name,
          phone: form.phone,
          date: form.date,
          time: form.time,
          zone: form.zone,
          service: form.service,
          notes: form.notes,
          invoice: invoice,
          status: 'confirmed',
          createdAt: serverTimestamp(),
        });
      } catch (err) {
        console.warn('Failed to save confirmed booking:', err);
      }
    }
    // Open WhatsApp with prefilled message
    window.open(whatsappLinkFromForm(form), '_blank');
  }

  function downloadInvoicePdf() {
    const doc = new jsPDF();
    doc.setFontSize(16);
    doc.text('Sacmoxs Barber — Invoice', 14, 20);
    doc.setFontSize(11);
    doc.text(`Client: ${form.name || '—'}`, 14, 30);
    doc.text(`Phone: ${form.phone || '—'}`, 14, 36);
    doc.text(`Date: ${form.date || '—'}   Time: ${form.time || '—'}`, 14, 42);
    let y = 55;
    invoice.items.forEach((it) => {
      doc.text(`${it.desc} — ${ghs(it.total)}`, 14, y);
      y += 8;
    });
    doc.text(`Total: ${ghs(invoice.total)}`, 14, y + 6);
    doc.save('sacmoxs-invoice.pdf');
  }

  async function markAsDone(id) {
    if (!db) return;
    try {
      await updateDoc(doc(db, 'bookings', id), { status: 'done' });
    } catch (err) {
      console.warn('Failed to update booking:', err);
    }
  }

  // Loyalty system example function (Phase 2)
  // Here we just return whether customer has >=5 completed bookings (needs DB)
  function eligibleForLoyalty(phone) {
    if (!bookings || bookings.length === 0) return false;
    const userBookings = bookings.filter((b) => b.phone === phone && b.status === 'done');
    return userBookings.length >= 5;
  }

  return (
    <div className="min-h-screen bg-gray-900 text-gray-100 p-6">
      <div className="max-w-6xl mx-auto">
        <header className="flex items-center justify-between mb-6">
          <div className="flex items-center gap-4">
            <div className="w-12 h-12 rounded-full bg-yellow-500 text-gray-900 flex items-center justify-center font-bold">S</div>
            <div>
              <h1 className="text-2xl font-bold">Sacmoxs Barber</h1>
              <div className="text-sm text-gray-400">{SERVICE_CITY}</div>
            </div>
          </div>
          <div className="flex gap-3 items-center">
            <button onClick={() => setAdminMode((s) => !s)} className="px-3 py-2 rounded bg-gray-800 border">{adminMode ? 'Client View' : 'Admin'}</button>
            <a href={whatsappLinkFromForm(form)} target="_blank" rel="noreferrer" className="bg-green-600 px-4 py-2 rounded">WhatsApp</a>
          </div>
        </header>

        {!adminMode ? (
          <main>
            <section className="grid grid-cols-1 md:grid-cols-2 gap-6 mb-6">
              <div>
                <h2 className="text-3xl font-bold mb-2">Luxury Grooming at Your Doorstep</h2>
                <p className="text-gray-400 mb-4">Professional, hygienic, and punctual mobile barbering across Accra. Book easily via the form or WhatsApp.</p>

                <div className="bg-gray-800 p-4 rounded mb-4">
                  <h3 className="font-semibold">Services & Pricing</h3>
                  <ul className="text-sm text-gray-400 mt-2">
                    <li>Haircut — {ghs(SERVICE_PRICES.haircut)}</li>
                    <li>Hair + Beard — {ghs(SERVICE_PRICES.haircut + SERVICE_PRICES.beard)}</li>
                    <li>Father & Son — {ghs(SERVICE_PRICES.fatherAndSon)}</li>
                    <li>Treatment — {ghs(SERVICE_PRICES.treatment)}</li>
                    <li>Student — {ghs(SERVICE_PRICES.student)} (within 3 km)</li>
                  </ul>
                </div>

                <div className="bg-gray-800 p-4 rounded">
                  <h3 className="font-semibold">How Booking Works</h3>
                  <ol className="text-sm text-gray-400 mt-2 list-decimal list-inside">
                    <li>Fill form and preview invoice.</li>
                    <li>Confirm booking and send WhatsApp message to Sacmoxs.</li>
                    <li>Pay via Mobile Money on arrival (or set up online pay later).</li>
                  </ol>
                </div>
              </div>

              <div className="bg-gray-800 p-4 rounded">
                <h3 className="font-semibold mb-2">Booking Form</h3>
                <form onSubmit={submitBooking} className="space-y-3">
                  <input value={form.name} onChange={(e) => updateForm({ name: e.target.value })} placeholder="Full Name" className="w-full p-2 bg-gray-700 rounded" required />
                  <input value={form.phone} onChange={(e) => updateForm({ phone: e.target.value })} placeholder="Phone (e.g. 024...)" className="w-full p-2 bg-gray-700 rounded" required />
                  <div className="grid grid-cols-2 gap-2">
                    <input type="date" value={form.date} onChange={(e) => updateForm({ date: e.target.value })} className="p-2 bg-gray-700 rounded" required />
                    <input type="time" value={form.time} onChange={(e) => updateForm({ time: e.target.value })} className="p-2 bg-gray-700 rounded" required />
                  </div>

                  <div>
                    <label className="block text-sm mb-1">Location / Zone</label>
                    <div className="grid grid-cols-3 gap-2">
                      {Object.entries(ZONES).map(([k, z]) => (
                        <label key={k} className={`p-2 rounded bg-gray-700 cursor-pointer ${form.zone === k ? 'ring-2 ring-yellow-500' : ''}`}>
                          <input type="radio" checked={form.zone === k} onChange={() => updateForm({ zone: k })} className="mr-2" />
                          <span className="font-semibold">{z.label}</span>
                          <div className="text-xs text-gray-400">Fee: GHS {z.fee}</div>
                        </label>
                      ))}
                    </div>

                  </div>

                  <div>
                    <label className="block text-sm mb-1">Services</label>
                    <div className="grid grid-cols-2 gap-2">
                      <label className="p-2 bg-gray-700 rounded cursor-pointer">
                        <input type="checkbox" checked={form.service.haircut} onChange={() => toggleService('haircut')} className="mr-2" /> Haircut (GHS 50)
                      </label>
                      <label className="p-2 bg-gray-700 rounded cursor-pointer">
                        <input type="checkbox" checked={form.service.beard} onChange={() => toggleService('beard')} className="mr-2" /> Beard (GHS 20)
                      </label>
                      <label className="p-2 bg-gray-700 rounded cursor-pointer">
                        <input type="checkbox" checked={form.service.treatment} onChange={() => toggleService('treatment')} className="mr-2" /> Treatment (GHS 30)
                      </label>
                      <label className="p-2 bg-gray-700 rounded cursor-pointer">
                        <input type="checkbox" checked={form.service.fatherAndSon} onChange={() => toggleService('fatherAndSon')} className="mr-2" /> Father & Son (GHS 100)
                      </label>
                      <label className="p-2 bg-gray-700 rounded cursor-pointer">
                        <input type="checkbox" checked={form.service.student} onChange={() => toggleService('student')} className="mr-2" /> Student (GHS 45)
                      </label>
                    </div>
                  </div>

                  <textarea value={form.notes} onChange={(e) => updateForm({ notes: e.target.value })} placeholder="Notes (e.g. low fade, clean line)" className="w-full p-2 bg-gray-700 rounded" />

                  <div className="flex gap-2">
                    <button type="submit" className="bg-yellow-500 text-gray-900 px-4 py-2 rounded">Preview Invoice</button>
                    <a className="border px-4 py-2 rounded" href={whatsappLinkFromForm(form)} target="_blank" rel="noreferrer">Message on WhatsApp</a>
                  </div>
                </form>

                {invoice && (
                  <div className="mt-4 bg-gray-800 p-3 rounded">
                    <h4 className="font-bold">Invoice Preview</h4>
                    <div className="text-sm text-gray-400">Subtotal: {ghs(invoice.subtotal)}</div>
                    <div className="text-sm text-gray-400">Total: {ghs(invoice.total)}</div>
                    <div className="flex gap-2 mt-2">
                      <button onClick={confirmAndSend} className="bg-green-600 px-3 py-2 rounded">Confirm & WhatsApp</button>
                      <button onClick={downloadInvoicePdf} className="border px-3 py-2 rounded">Download PDF</button>
                    </div>
                  </div>
                )}

                {message && <div className="mt-3 text-sm text-green-400">{message}</div>}
              </div>
            </section>

            <section className="mb-6">
              <h3 className="text-xl font-bold mb-2">Gallery</h3>
              <div className="grid grid-cols-2 sm:grid-cols-4 gap-2">
                {[1, 2, 3, 4, 5, 6].map((i) => (
                  <div key={i} className="h-32 bg-gray-800 rounded overflow-hidden">
                    <img alt={`g-${i}`} src={`https://images.unsplash.com/photo-1520975698513-6f2b2f8a9f31?auto=format&fit=crop&w=600&q=60`} className="w-full h-full object-cover" />
                  </div>
                ))}
              </div>
            </section>

            <section className="mb-6">
              <h3 className="text-xl font-bold mb-2">Reviews</h3>
              <div className="grid grid-cols-1 sm:grid-cols-3 gap-2">
                <div className="bg-gray-800 p-3 rounded">"Great fade — came on time." — Kwame</div>
                <div className="bg-gray-800 p-3 rounded">"Professional and neat setup." — Yaw</div>
                <div className="bg-gray-800 p-3 rounded">"Father and son package was perfect." — Ama</div>
              </div>
            </section>

            <section>
              <h3 className="text-xl font-bold mb-2">Service Areas</h3>
              <div className="bg-gray-800 p-3 rounded mb-2">Based in {SERVICE_CITY}. Distance fee applies based on selected zone.</div>
              <div className="w-full h-48 rounded overflow-hidden">
                <iframe title="map" src="https://www.google.com/maps/embed?pb=!1m18!1m12!1m3!1d3972.0!2d-0.0!3d5.6037!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1" className="w-full h-full border-0"></iframe>
              </div>
            </section>

          </main>
        ) : (
          <AdminDashboard bookings={bookings} onMarkDone={markAsDone} />
        )}

        <footer className="mt-6 text-sm text-gray-400 flex justify-between">
          <div>© {new Date().getFullYear()} Sacmoxs Barber</div>
          <div>Contact: {BUSINESS_PHONE} — Book via WhatsApp</div>
        </footer>
      </div>
    </div>
  );
}

// ---------------- Admin Dashboard Component ----------------
function AdminDashboard({ bookings = [], onMarkDone = () => {} }) {
  return (
    <div className="bg-gray-800 p-4 rounded">
      <h2 className="text-2xl font-bold mb-3">Admin Dashboard</h2>
      <div className="text-sm text-gray-400 mb-3">Recent bookings (live if Firebase configured)</div>
      <div className="space-y-2">
        {bookings.length === 0 && <div className="text-gray-500">No bookings yet.</div>}
        {bookings.map((b) => (
          <div key={b.id} className="p-3 bg-gray-700 rounded flex justify-between items-center">
            <div>
              <div className="font-semibold">{b.name} — {b.phone}</div>
              <div className="text-xs text-gray-400">{b.date} {b.time} • {b.zone} • {b.status}</div>
              <div className="text-xs mt-1">Total: {b.invoice ? ghs(b.invoice.total) : '—'}</div>
            </div>
            <div className="flex gap-2">
              <button onClick={() => onMarkDone(b.id)} className="px-3 py-1 bg-yellow-500 rounded text-gray-900">Mark Done</button>
            </div>
          </div>
        ))}
      </div>
    </div>
  );
}
