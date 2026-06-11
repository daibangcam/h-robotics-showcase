import streamlit as st
import pandas as pd
import gspread
from google.oauth2.service_account import Credentials

# Giao diện wide mode tối ưu toàn màn hình
st.set_page_config(page_title="HUIT Robotics 2026", layout="wide", page_icon="🤖")

# ==========================================
# 🛠️ CSS TRỊ BỆNH SAFARI, CĂN GIỮA BẢNG & TỰ ĐỘNG BẺ 2 HÀNG TAB
# ==========================================
st.markdown("""
    <style>
    /* TRỊ BỆNH ZOOM CỦA SAFARI */
    input, select, textarea, div[data-baseweb="select"] * {
        font-size: 16px !important;
    }

    /* Chống phình khung */
    div[data-testid="column"], div[data-testid="stVerticalBlock"] {
        min-width: 0 !important;
    }

    /* 🌟 YÊU CẦU 1: ÉP THANH SUB-TABS TỰ ĐỘNG XUỐNG HÀNG (CHIA THÀNH 2-3 ROW TỰ ĐỘNG) */
    div[data-baseweb="tab-list"] {
        flex-wrap: wrap !important;
        gap: 5px !important;
    }
    div[data-baseweb="tab"] {
        padding: 6px 12px !important;
        border-radius: 4px !important;
    }

    /* Thiết kế bảng phong cách tối giản căn giữa */
    .huit-table {
        width: 100%;
        border-collapse: collapse;
        margin-bottom: 20px;
        font-family: 'Arial', sans-serif;
    }
    .huit-table th {
        background-color: #f1f2f6 !important;
        color: #2f3542 !important;
        font-weight: bold;
        padding: 12px;
        border: 1px solid #ced4da;
        text-align: center !important;
    }
    .huit-table td {
        padding: 12px;
        border: 1px solid #ced4da;
        color: #2f3542 !important;
        text-align: center !important;
        background-color: #ffffff !important;
    }

    /* Làm đẹp thanh chọn Vòng đấu ngang */
    div.row-widget.stRadio > div {
        flex-direction: row;
        justify-content: center;
        background-color: #f8f9fa;
        padding: 10px;
        border-radius: 10px;
        box-shadow: 0 2px 4px rgba(0,0,0,0.05);
    }
    </style>
""", unsafe_allow_html=True)

# ==========================================
# 🎛️ MENU CHỌN VÒNG ĐẤU NẰM NGANG TRÊN HEADER
# ==========================================
vong_thi_dau = st.radio(
    "Chọn vòng thi đấu:",
    ["VÒNG LOẠI", "VÒNG CHUNG KẾT"],
    index=1,  # 🌟 YÊU CẦU 3: MẶC ĐỊNH CHỌN VÒNG CHUNG KẾT KHI MỞ WEB
    horizontal=True,
    label_visibility="collapsed"
)

# Khai báo Sheet và Tiêu đề theo Vòng đấu
if vong_thi_dau == "VÒNG LOẠI":
    sheet_doi = "Đội đấu VL"
    sheet_lich = "Lịch đấu VL"
    title_text = "VÒNG LOẠI"
else:
    sheet_doi = "Đội đấu VCK"
    sheet_lich = "Lịch đấu VCK"
    title_text = "VÒNG CHUNG KẾT"

# ==========================================
# HEADER HUIT DYNAMIC (TỰ ĐỘNG THAY ĐỔI TÊN VÒNG ĐẤU)
# ==========================================
st.markdown(f"""
    <div style="background-color: #4b7bec; padding: 12px 15px; border-radius: 8px; text-align: center; margin-bottom: 20px; border-left: 8px solid #fa8231;">
        <h5 style="color: #ffffff; margin: 0; padding: 0; line-height: 1.2; font-family: 'Arial'; font-weight: bold; font-size: 13px; letter-spacing: 0.5px;">TRƯỜNG ĐẠI HỌC CÔNG THƯƠNG TP. HCM</h5>
        <h6 style="color: #ffffff; margin: 2px 0 0 0; padding: 0; line-height: 1.2; font-family: 'Arial'; font-weight: normal; font-size: 13px;">KHOA CÔNG NGHỆ ĐIỆN - ĐIỆN TỬ</h6>
        <h6 style="color: #ffffff; margin: 1px 0 0 0; padding: 0; line-height: 1.2; font-family: 'Arial'; font-weight: normal; font-size: 13px;">BỘ MÔN TỰ ĐỘNG HÓA</h6>
        <h3 style="color: #fa8231; margin: 10px 0 4px 0; padding: 0; line-height: 1.3; font-family: 'Arial'; font-weight: bold; font-size: 17px; letter-spacing: 1px;">
            CUỘC THI ROBOTICS 2026<br>{title_text}
        </h3>
        <p style="color: #ffffff; margin: 4px 0 0 0; padding: 0; line-height: 1.2; font-size: 13px; font-style: italic;">
            Developed by <a href="https://www.tiktok.com/@h2q.solution" target="_blank" style="color: #4cd137; font-weight: bold; text-decoration: none;">H2Q Solution</a>  &  <a href="https://hano.cf" target="_blank" style="color: #4cd137; font-weight: bold; text-decoration: none;">hano.cf</a>
        </p>
    </div>
""", unsafe_allow_html=True)


# --- KẾT NỐI DATABASE ---
@st.cache_resource
def init_connection():
    creds = Credentials.from_service_account_info(st.secrets["gcp_service_account"],
                                                  scopes=["https://www.googleapis.com/auth/spreadsheets"])
    client = gspread.authorize(creds)
    return client.open_by_key(st.secrets["database"]["SHEET_ID"])


try:
    conn = init_connection()
except Exception as e:
    st.error(f"Lỗi kết nối API Google Sheets: {e}")
    st.stop()


# Cập nhật cache: Nếu đổi vòng đấu, hàm sẽ bốc đúng sheet của vòng đó
@st.cache_data(ttl=1)
def load_fresh_data(sheet_teams_name, sheet_matches_name):
    teams = pd.DataFrame(conn.worksheet(sheet_teams_name).get_all_records())
    matches = pd.DataFrame(conn.worksheet(sheet_matches_name).get_all_records())
    teams.columns = ['Bảng', 'Đội']
    matches.columns = ['Bảng', 'Đội 1', 'Đội 2', 'Kết quả']
    return teams, matches


teams, matches = load_fresh_data(sheet_doi, sheet_lich)


# --- HÀM TÍNH TOÁN ĐIỂM SỐ ---
def calculate_tournament_stats(teams, matches):
    stats = teams.copy()
    stats['Thắng'] = 0
    stats['Hòa'] = 0
    stats['Thua'] = 0
    stats['Điểm'] = 0
    stats['Số trận'] = 0

    for _, row in matches.iterrows():
        kq = str(row['Kết quả']).strip()
        if kq not in ['1', '2', '0']: continue

        d1, d2 = str(row['Đội 1']).strip(), str(row['Đội 2']).strip()
        idx1 = stats[stats['Đội'] == d1].index
        idx2 = stats[stats['Đội'] == d2].index

        if not idx1.empty and not idx2.empty:
            stats.loc[idx1, 'Số trận'] += 1
            stats.loc[idx2, 'Số trận'] += 1
            if kq == "1":
                stats.loc[idx1, ['Thắng', 'Điểm']] += [1, 3]
                stats.loc[idx2, 'Thua'] += 1
            elif kq == "2":
                stats.loc[idx2, ['Thắng', 'Điểm']] += [1, 3]
                stats.loc[idx1, 'Thua'] += 1
            elif kq == "0":
                stats.loc[idx1, ['Hòa', 'Điểm']] += [1, 1]
                stats.loc[idx2, ['Hòa', 'Điểm']] += [1, 1]
    return stats


stats = calculate_tournament_stats(teams, matches)

colors = {
    'A': '#FF9999', 'B': '#66B2FF', 'C': '#99FF99', 'D': '#FFCC99',
    'E': '#FF99FF', 'F': '#CC99FF', 'G': '#99FFFF', 'H': '#FFFF99', 'I': '#D3D3D3',
    'Chung kết': '#ffeaa7', 'Chung Kết': '#ffeaa7'
}
bang_list = sorted(teams['Bảng'].unique())

# Khởi tạo 3 Tab lớn ngoài cùng
tab1, tab2, tab3 = st.tabs(["📅 Lịch thi đấu", "📊 Bảng xếp hạng", "⚖️ Ban giám khảo"])


# Hàm ép render HTML ép căn lề giữa tuyệt đối cho bảng biểu công cộng
def display_html_table(df, columns_mapping):
    df_render = df.copy()
    if columns_mapping:
        df_render = df_render.rename(columns=columns_mapping)
    html = df_render.to_html(index=False, classes='huit-table')
    st.markdown(html, unsafe_allow_html=True)


# ==========================================
# 1. TAB LỊCH THI ĐẤU
# ==========================================
with tab1:
    if bang_list:
        sub_tabs_ltd = st.tabs([f"🔹 {b}" for b in bang_list])

        for sub_tab, bang in zip(sub_tabs_ltd, bang_list):
            with sub_tab:
                df_m = matches[matches['Bảng'] == bang].copy().reset_index(drop=True)
                df_m.insert(0, "Trận", df_m.index + 1)


                def format_display_result(r):
                    val = str(r['Kết quả']).strip()
                    if val == "1": return f"🏆 {r['Đội 1']} Thắng"
                    if val == "2": return f"🏆 {r['Đội 2']} Thắng"
                    if val == "0": return "🤝 Hòa"
                    return "⏳ Chưa đấu"


                df_m['Kết quả hiển thị'] = df_m.apply(format_display_result, axis=1)
                final_df = df_m[['Trận', 'Đội 1', 'Đội 2', 'Kết quả hiển thị']]

                ten_bang = bang.replace("Bảng ", "").strip()
                st.markdown(
                    f'<div style="background-color: {colors.get(ten_bang, "#f1f2f6")}; padding: 8px 12px; border-radius: 4px; color: black; font-weight: bold; margin-bottom: 8px; text-align: center;">KẾT QUẢ {bang.upper()}</div>',
                    unsafe_allow_html=True)

                display_html_table(final_df, {'Kết quả hiển thị': 'Kết quả'})

# ==========================================
# 2. TAB BẢNG XẾP HẠNG
# ==========================================
with tab2:
    if bang_list:
        sub_tabs_bxh = st.tabs([f"📈 {b}" for b in bang_list])

        for sub_tab, bang in zip(sub_tabs_bxh, bang_list):
            with sub_tab:
                df_bang = stats[stats['Bảng'] == bang].copy()
                df_bang = df_bang.sort_values(by=['Điểm', 'Đội'], ascending=[False, True])

                so_doi_trong_bang = len(df_bang)
                max_matches = so_doi_trong_bang - 1 if so_doi_trong_bang > 0 else 0

                df_bang['Tiến độ'] = df_bang['Số trận'].astype(str) + f"/{max_matches}"
                df_bang = df_bang.drop(columns=['Bảng', 'Số trận'])

                ten_bang = bang.replace("Bảng ", "").strip()
                st.markdown(
                    f'<div style="background-color: {colors.get(ten_bang, "#f1f2f6")}; padding: 8px 12px; border-radius: 4px; color: black; font-weight: bold; margin-bottom: 8px; text-align: center;">XẾP HẠNG {bang.upper()}</div>',
                    unsafe_allow_html=True)

                display_html_table(df_bang, {})

# ==========================================
# 3. TAB BAN GIÁM KHẢO
# ==========================================
with tab3:
    if "last_pw" not in st.session_state:
        st.session_state.last_pw = ""

    pw = st.text_input("Nhập mật khẩu quyền Ban giám khảo:", type="password")

    if pw != st.session_state.last_pw:
        if pw == st.secrets["auth"]["BGK_PASSWORD"]:
            st.toast("✅ BGK đăng nhập thành công", icon="ℹ️")
        elif pw != "":
            st.toast("❌ Mật khẩu không hợp lệ!", icon="⚠️")
        st.session_state.last_pw = pw

    if pw == st.secrets["auth"]["BGK_PASSWORD"]:
        if bang_list:
            sub_tabs_bgk = st.tabs([f"⚖️ {b}" for b in bang_list])

            for sub_tab, bang in zip(sub_tabs_bgk, bang_list):
                with sub_tab:
                    df_m = matches[matches['Bảng'] == bang].reset_index()

                    ten_bang = bang.replace("Bảng ", "").strip()
                    st.markdown(
                        f'<div style="background-color: {colors.get(ten_bang, "#f1f2f6")}; padding: 8px 12px; border-radius: 4px; color: black; font-weight: bold; margin-bottom: 15px; text-align: center;">ĐIỀU KHIỂN TRẬN ĐẤU - {bang.upper()}</div>',
                        unsafe_allow_html=True)

                    for i, row in df_m.iterrows():
                        c1, c2 = st.columns([1, 1])

                        # 🌟 YÊU CẦU 2: ĐỔI MÀU CHỮ THÀNH ĐEN TUYỀN (#4cd137) ĐẬM ĐÀ RÕ NÉT, TĂNG SIZE 16PX CHIẾN DI ĐỘNG
                        c1.markdown(f"""
                            <div style='margin-top: 5px; margin-bottom: 8px;'>
                                <span style='background-color: #6c5ce7; color: white; padding: 4px 10px; border-radius: 4px; font-weight: bold; font-size: 13px; margin-right: 8px;'>Trận {i + 1}</span>
                                <span style='font-weight: bold; font-size: 16px; color: #4cd137;'>{row['Đội 1']} 🆚 {row['Đội 2']}</span>
                            </div>
                        """, unsafe_allow_html=True)

                        d1, d2 = str(row['Đội 1']).strip(), str(row['Đội 2']).strip()
                        options = ["⏳ Chưa chọn kết quả", f"🏆 {d1} Thắng", "🤝 Hòa", f"🏆 {d2} Thắng"]
                        val_mapping = {"⏳ Chưa chọn kết quả": "", f"🏆 {d1} Thắng": "1", "🤝 Hòa": "0",
                                       f"🏆 {d2} Thắng": "2"}

                        curr_val = str(row['Kết quả']).strip()
                        if curr_val == "1":
                            curr_display = f"🏆 {d1} Thắng"
                        elif curr_val == "2":
                            curr_display = f"🏆 {d2} Thắng"
                        elif curr_val == "0":
                            curr_display = "🤝 Hòa"
                        else:
                            curr_display = "⏳ Chưa chọn kết quả"

                        idx_opt = options.index(curr_display) if curr_display in options else 0

                        selection = c2.selectbox(
                            f"Kết quả trận {row['index']}",
                            options,
                            index=idx_opt,
                            label_visibility="collapsed",
                            key=f"bgk_drop_{sheet_lich}_{row['index']}"
                        )

                        new_db_val = val_mapping[selection]
                        if new_db_val != curr_val:
                            with st.spinner("🔄 Đang đồng bộ..."):
                                conn.worksheet(sheet_lich).update_cell(int(row['index']) + 2, 4, new_db_val)
                                st.cache_data.clear()
                            st.rerun()

                        st.markdown("<hr style='margin: 15px 0; border: none; border-top: 1px dashed #ced4da;'/>",
                                    unsafe_allow_html=True)