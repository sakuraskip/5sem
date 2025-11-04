using aspa0011_1.Models;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Orleans.TestingHost.Logging;
namespace aspa0011_1
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var builder = WebApplication.CreateBuilder(args);

            builder.Logging.ClearProviders();
            builder.Logging.AddConsole();
            builder.Logging.AddDebug();

            if (builder.Environment.IsDevelopment())
            {
                builder.Logging.AddFilter("Microsoft", LogLevel.Trace);
                builder.Logging.AddFilter("System", LogLevel.Trace);
                builder.Logging.AddFilter("aspa0011_1", LogLevel.Trace);
            }
            else
            {
                builder.Logging.SetMinimumLevel(LogLevel.Information);
                builder.Logging.AddFilter("Microsoft", LogLevel.Warning);
                builder.Logging.AddFilter("System", LogLevel.Warning);
                builder.Logging.AddFilter("aspa0011_1", LogLevel.Information);
            }

            // Add services to the container.
            builder.Services.AddControllers();
            builder.Services.AddControllersWithViews();

            builder.Services.AddEndpointsApiExplorer();
            builder.Services.AddSwaggerGen();

            builder.Services.AddSingleton<ChannelService>();
            //builder.Services.AddSingleton<ILogger<ChannelService>>();



            builder.Logging.AddFile("./Logs/aspa0011_1.txt");

            var app = builder.Build();

            // Configure the HTTP request pipeline.
            if (!app.Environment.IsDevelopment())
            {
                app.UseExceptionHandler("/Home/Error");
                // The default HSTS value is 30 days. You may want to change this for production scenarios, see https://aka.ms/aspnetcore-hsts.
                app.UseHsts();
            }
            if (app.Environment.IsDevelopment())
            {
                app.UseSwagger();
                app.UseSwaggerUI(c =>
                {
                    c.SwaggerEndpoint("/swagger/v1/swagger.json", "v1");
                });

            }


            app.UseHttpsRedirection();
            app.UseStaticFiles();

            app.UseRouting();

            app.UseAuthorization();

            app.MapControllerRoute(
                name: "default",
                pattern: "{controller=Home}/{action=Index}/{id?}");

            app.Run();
        }
    }
}
